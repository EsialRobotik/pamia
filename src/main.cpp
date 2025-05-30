#include <Arduino.h>
#include "CommandManager.hpp"
#include "IODebouncer.h"
#include "PamiHardware.hpp"
#include "TestManager.hpp"
#include "PamIA.hpp"
#include "AsservManager.hpp"
#include "Chrono.hpp"
#include "DetectionManager.hpp"
#include "RoadMap.hpp"
#include <vector>
#include <ESP32Servo.h>

#define PIN_LIDAR_PWM 18
#define PWM_CHANNEL 0
#define PWM_FREQ 1000
#define PWM_RESOLUTION 10

#define UART_PC_SPEED 115200
#define UART_NUCLEO 115200

#define IO_DEBOUNCE_PERIOD 5

#define IO_ID_TIRETTE 0
#define IO_ID_COULEUR 1
#define IO_ID_GRPIO_1 2
#define IO_ID_GRPIO_2 3
#define IO_ID_GPIO_SERVO_01 4
#define IO_ID_GPIO_SERVO_02 5
#define IO_I2C_SDA 8
#define IO_I2C_SCL 7

#define STOP_MOVING_MS 1000000 // TODO valeurs de test
#define DETECTION_THRESHOLD_CM 13

#define SERVO_POS_LOW 50
#define SERVO_POS_HIGH 130
#define SERVO_OSCILLATION_PERIOD 2000

#define PAMI_STAR_SRF_CODE 127
#define PAMI_GROUPIE_1_SRF_CODE 254
#define PAMI_GROUPIE_2_SRF_CODE 253
#define PAMI_GROUPIE_3_SRF_CODE 251

// Pour chaque PAMI, on associe une feuille de route à une adresse de SRF08
// RoadMapItem { command, x, y, angle, detectionThresholdOrPeriod }

#define CHOREE_OFFSET 14150
#define SYNC_CHOREE_0 0
#define SYNC_CHOREE_1 SYNC_CHOREE_0 + CHOREE_OFFSET
#define SYNC_CHOREE_2 SYNC_CHOREE_1 + CHOREE_OFFSET
#define SYNC_CHOREE_3 SYNC_CHOREE_2 + CHOREE_OFFSET
#define SYNC_CHOREE_4 SYNC_CHOREE_3 + CHOREE_OFFSET
#define SYNC_CHOREE_5 SYNC_CHOREE_4 + CHOREE_OFFSET
#define SYNC_CHOREE_6 SYNC_CHOREE_5 + CHOREE_OFFSET
#define SYNC_CHOREE_7 SYNC_CHOREE_6 + CHOREE_OFFSET
#define TURN_ANGLE 315
#define DISTANCE 150

std::vector<RoadMapItem> CHOREE = {
  // au début seule la star danse
  {ROADMAP_COMMAND::DISABLE_ASSERV, PAMI_GROUPIE_1_SRF_CODE, 0, 0, 0},
  {ROADMAP_COMMAND::DISABLE_ASSERV, PAMI_GROUPIE_2_SRF_CODE, 0, 0, 0},
  {ROADMAP_COMMAND::DISABLE_ASSERV, PAMI_GROUPIE_3_SRF_CODE, 0, 0, 0},

    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_0, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_0 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_0 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_0 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_0 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_0 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_0 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},

    // PAMI 1 ON
    {ROADMAP_COMMAND::ENABLE_ASSERV, PAMI_GROUPIE_1_SRF_CODE, 0, 0, 0},

    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_1, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_1 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_1 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_1 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_1 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_1 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_1 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},


    // PAMI 2 ON
    {ROADMAP_COMMAND::ENABLE_ASSERV, PAMI_GROUPIE_2_SRF_CODE, 0, 0, 0},
    
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_2, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_2 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_2 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_2 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_2 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_2 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_2 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},

      // PAMI 3 ON
    {ROADMAP_COMMAND::ENABLE_ASSERV, PAMI_GROUPIE_3_SRF_CODE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_3, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_3 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_3 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_3 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_3 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_3 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_3 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},

        {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_4, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_4 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_4 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_4 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_4 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_4 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_4 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},

        {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},

    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_5 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},

    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_6, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_6 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_6 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_6 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_6 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_6 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_6 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},

    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_7, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_7 + 1900, 0, 0, 0}, // Doigts derrière
    {ROADMAP_COMMAND::GO, -DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_7 + 3800, 0, 0, 0}, // Doigts devant
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_7 + 5200, 0, 0, 0}, // tout petits ronds
    {ROADMAP_COMMAND::CELEBRATE, DISTANCE, 130, 0, 200},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_7 + 7300, 0, 0, 0}, // boogie boogie
    {ROADMAP_COMMAND::STOP_CELEBRATE, 0, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, -35, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 70, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_7 + 8800, 0, 0, 0}, // tour de moi même
    {ROADMAP_COMMAND::TURN, TURN_ANGLE, 0, 0, 0},
    {ROADMAP_COMMAND::SYNC_TO_MATCH_TIME, SYNC_CHOREE_7 + 10800, 0, 0, 0}, // je vais en avant 
    {ROADMAP_COMMAND::GO, DISTANCE, 0, 0, 0},
    {ROADMAP_COMMAND::TURN, 90, 0, 0, 0},

  };

std::vector<RoadMap> roadMaps = {
  // GROUPIE 1
  RoadMap(PAMI_GROUPIE_1_SRF_CODE, PAMIA_COLOR::YELLOW, CHOREE),
  
  // GROUPIE 2
  RoadMap(PAMI_GROUPIE_2_SRF_CODE, PAMIA_COLOR::YELLOW, CHOREE),
  
  // GROUPIE 3
  RoadMap(PAMI_GROUPIE_3_SRF_CODE, PAMIA_COLOR::YELLOW, CHOREE),
  
  // STAR
  RoadMap(PAMI_STAR_SRF_CODE, PAMIA_COLOR::YELLOW, CHOREE),
};

IODebouncer btnTirette(IO_ID_TIRETTE, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnCouleur(IO_ID_COULEUR, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnGpio1(IO_ID_GRPIO_1, IO_DEBOUNCE_PERIOD, false);
IODebouncer btnGpio2(IO_ID_GRPIO_2, IO_DEBOUNCE_PERIOD, false);
SRF08 srf08(0, 1000); // On ne sait pas quel SRF08 est connecté au boot : adresse 0 en attendant
Servo servo1;

PamiHardWare pamiHardware(&btnTirette, &btnCouleur, &btnGpio1, &btnGpio2, &srf08, &servo1);
TestManager testManager(&Serial, &pamiHardware);
AsservManager asservManager(&Serial0, &Serial);
DetectionManager detectionManager(&pamiHardware);
PamIA pamiA(&Serial, &pamiHardware, &asservManager, &detectionManager, roadMaps, STOP_MOVING_MS);
CommandManager commandManager(&Serial, &testManager, &pamiA);

void setup() {
  Serial.begin(UART_PC_SPEED);
  Serial.setTimeout(10);
  Serial0.begin(UART_NUCLEO);
  Serial0.setTimeout(10);
  servo1.attach(IO_ID_GPIO_SERVO_01);
  Wire.begin(IO_I2C_SDA, IO_I2C_SCL, 100000);

  delay(2000);
  Serial.println("Detection du SRF08 branche...");
  for (int i=0; i<roadMaps.size(); i++) {
    Wire.beginTransmission(roadMaps[i].getSrfAddress());
    if (Wire.endTransmission() == 0) {
      srf08.setAddress(roadMaps[i].getSrfAddress());
      break;
    }
  }
  if (srf08.getAddress() == 0) {
    Serial.println("SRF08 non trouve!");
  } else {
    Serial.print("SRF08 trouve : ");
    Serial.println(srf08.getAddress());
  }
}

void loop() {
  commandManager.heartBeat();
  testManager.heartBeat();
  asservManager.heartBeat();
  detectionManager.heartBeat();
  pamiA.heartBeat();
}
