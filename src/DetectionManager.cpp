#include "DetectionManager.hpp"

DetectionManager::DetectionManager(PamiHardWare* pamiHardware)
: pamiHardware(pamiHardware)
, lastDistance(0)
{
}

int DetectionManager::getObstacleDistance() {
    return lastDistance;
}

void DetectionManager::heartBeat() {
    if (pamiHardware->srf08->getAddress() == 0) {
        return;
    }
    if (pamiHardware->srf08->checkMeasureResponse()) {
        lastDistance = pamiHardware->srf08->getLastMeasureCentimeter();
        pamiHardware->srf08->triggerMeasure();
    }

    if (lastDistance == 0) {
        lastDistance = 1;
        pamiHardware->srf08->triggerMeasure();
    }
}