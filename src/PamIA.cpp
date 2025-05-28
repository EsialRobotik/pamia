#include "PamIA.hpp"

PamIA::PamIA(Stream* serial, PamiHardWare* pamiHardware, AsservManager* asservManager, DetectionManager* detectionManager, std::vector<RoadMap> roadMaps, unsigned long matchDuration)
: serial(serial)
, pamiHardware(pamiHardware)
, asservManager(asservManager)
, detectionManager(detectionManager)
, roadMaps(roadMaps)
, state(PAMIA_STATE::WAIT_TIRETTE_PLUG)
, chronoStopMoving(Chrono(matchDuration))
, chronoStateTicker(Chrono(1000))
, chronoCelebration(Chrono(0))
, obstacleDetected(false)
, lastCommandStartTime(0)
, celebrateServoMin(0)
, celebrateServoMax(0)
{
}

void PamIA::heartBeat() {
  switch (state) {
    case WAIT_TIRETTE_PLUG:
      if (pamiHardware->pinTirette->isPressed()) {
        state = WAITING_TIRETTE_UNPLUG;
        serial->println("Tirette inseree, construction roadmap...");
        PAMIA_COLOR color = pamiHardware->pinCouleur->isOn() ? PAMIA_COLOR::BLUE : PAMIA_COLOR::YELLOW;
        roadMapIndexToUse = -1;
        for (int i=0; i<roadMaps.size(); i++) {
          if (roadMaps[i].getSrfAddress() == pamiHardware->srf08->getAddress() && roadMaps[i].getColor() == color) {
            roadMapIndexToUse = i;
            break;
          }
        }
        if (roadMapIndexToUse == -1) {
          serial->print("Pas de roadmap pour le srf a l'adresse ");
          serial->println(pamiHardware->srf08->getAddress());
        } else {
          serial->print("Utilisation roadmap n. ");
          serial->print(roadMapIndexToUse);
          serial->print(" pour le srf a l'adresse ");
          serial->println(pamiHardware->srf08->getAddress());
        }
      }
      break;
    case WAITING_TIRETTE_UNPLUG:
      if (pamiHardware->pinTirette->isReleased()) {
        if (roadMapIndexToUse == -1) {
          serial->println("No roadmap found !");
          state = NO_ROAD_MAP;
        } else {
          serial->println("Tirette retiree, chrono deplacement...");
          state = RUN_ROADMAP;
          chronoStopMoving.start();
          nextCommand(); // Amorçage de la roadmap
        }
      }
      break;
    case RUN_ROADMAP:
      // Si le chrono est terminé ou que la liste des commandes à exécuter est terminée : stop
      if (chronoStopMoving.isElapsed() || (currentCommandHeartBeat() && !nextCommand())) {
        serial->println("Chrono match termine");
        state = END;
      }
      break;
    case END:
      break;
  }

  if (chronoCelebration.triggered()) {
    if (chronoCelebration.isEven()) {
      pamiHardware->servo1->write(celebrateServoMin);
    } else {
      pamiHardware->servo1->write(celebrateServoMax);
    }
  }

  if (chronoStateTicker.triggered()) {
    serial->print("Etat : ");
    serial->println(state);
  }
}

bool PamIA::nextCommand() {
  if (!roadMaps[roadMapIndexToUse].hasNextItem()) {
    serial->println("nextCommand() : no more command");
    return false;
  }
  RoadMapItem rmi = roadMaps[roadMapIndexToUse].getNextItem();
  switch (rmi.command) {
    case ROADMAP_COMMAND::GO_TO:
      serial->print("nextCommand() : goto ");
      serial->print(rmi.x);
      serial->print(";");
      serial->print(rmi.y);
      serial->print(" avec detection ");
      serial->print(rmi.detectionThresholdOrPeriod);
      serial->println("cm");
      asservManager->goTo(rmi.x, rmi.y);
      lastCommandStartTime = millis();
      break;
    case ROADMAP_COMMAND::GO:
      serial->print("nextCommand() : go ");
      serial->print(rmi.x);
      serial->print(" avec detection ");
      serial->print(rmi.detectionThresholdOrPeriod);
      serial->println("cm");
      asservManager->go(rmi.x);
      lastCommandStartTime = millis();
      break;
    case ROADMAP_COMMAND::SET_POSITION:
      serial->print("nextCommand() : set position ");
      serial->print(rmi.x);
      serial->print(";");
      serial->println(rmi.y);
      serial->print(";");
      serial->println(rmi.angle);
      asservManager->setPosition(rmi.x, rmi.y, rmi.angle);
      lastCommandStartTime = millis();
      break;
    case ROADMAP_COMMAND::WAIT:
      serial->print("nextCommand() : attente ");
      serial->print(rmi.x);
      serial->println("ms");
      lastCommandStartTime = millis();
      break;
    case ROADMAP_COMMAND::CELEBRATE:
      serial->print("nextCommand() : celebrate, period ");
      serial->print(rmi.detectionThresholdOrPeriod);
      serial->println("ms");
      celebrateServoMin = rmi.x;
      celebrateServoMax = rmi.y;
      chronoCelebration.setPeriod((unsigned long) rmi.detectionThresholdOrPeriod);
      chronoCelebration.start();
      lastCommandStartTime = millis();
    case ROADMAP_COMMAND::WAIT_TIRETTE_UNPLUG:
      serial->println("nextClommand() : wait tirette unplug");
      lastCommandStartTime = millis();
      break;
  }

  return true;
}

bool PamIA::currentCommandHeartBeat() {
  RoadMapItem rmi = roadMaps[roadMapIndexToUse].current();
  switch (rmi.command) {
    case ROADMAP_COMMAND::GO_TO:
      // On considère qu'on est arrivé à destination lorsque toutes les conditions suivantes sont réunies :
      // - on est pas bloqué
      // - le dernier tick d'asserv a été reçu après l'envoi/la reprise de la commande
      // - l'asserv indique qu'elles est idle
      if (!obstacleDetected && asservManager->getLastData().time > lastCommandStartTime && asservManager->asservIdle()) {
        serial->println("currentCommandHeartBeat() : goto termine");
        return true;
      }

      if (detectionManager->getObstacleDistance() < rmi.detectionThresholdOrPeriod) {
        if (!obstacleDetected) {
          serial->println("currentCommandHeartBeat() : obstacle detecte, arret urgence");
          asservManager->emergencyStop();
          obstacleDetected = true;
        }
      } else {
        if (obstacleDetected) {
          serial->println("currentCommandHeartBeat() : obstacle parti, reprise goto");
          obstacleDetected = false;
          asservManager->emergencyReset();
          delay(20);
          asservManager->goTo(rmi.x, rmi.y);
          lastCommandStartTime = millis();
        }
      }
      return false;
    case ROADMAP_COMMAND::GO:
      return asservManager->asservIdle();
    case ROADMAP_COMMAND::SET_POSITION:
      return millis() > (lastCommandStartTime + 5); // On laisse 5 ms à l'asserv pour assimiler la position 
    case ROADMAP_COMMAND::WAIT:
      if (millis() > lastCommandStartTime + (unsigned long) rmi.detectionThresholdOrPeriod) {
        serial->println("currentCommandHeartBeat() : attente terminee");
        return true;
      }
      return false;
    case ROADMAP_COMMAND::CELEBRATE:
      return true;
      break;
    case ROADMAP_COMMAND::WAIT_TIRETTE_UNPLUG:
      return pamiHardware->pinTirette->isJustReleased();
      break;
  }
  return true;
}