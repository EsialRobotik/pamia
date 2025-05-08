#include "TestManager.hpp"

TestManager::TestManager(Stream * serial, PamiHardWare* pamiHardware)
    : serial(serial)
    , pamiHardware(pamiHardware)
    , earsContinousTest(false)
    , earsStartTestTime(0)
{
}

bool TestManager::toggleIoTest()
{
    testIoOn = !testIoOn;
    return testIoOn;
}

void TestManager::testSrf08(bool continuous)
{
    pamiHardware->srf08->reset();
    if (!srf08TestContinuous) {
        serial->print("Demarrage mesure srf08 a l'adresse ");
        serial->print(pamiHardware->srf08->getAddress());
        serial->println("...");
    }
    if (pamiHardware->srf08->triggerMeasure()) {
        srf08TestEngaged = true;
        srf08TestContinuous = continuous;
    } else {
        srf08TestEngaged = false;
        srf08TestContinuous = false;
        serial->println("erreur : srf08 absent ?");
    }
}

void TestManager::heartBeat()
{
    if (testIoOn) {
        // Tirette
        if (pamiHardware->pinTirette->isJustPressed()) {
            serial->println("Tirette ON");
        } else if (pamiHardware->pinTirette->isJustReleased()) {
            serial->println("Tirette OFF");
        }

        // Couleur
        if (pamiHardware->pinCouleur->isJustPressed()) {
            serial->println("Couleur ON");
        } else if (pamiHardware->pinCouleur->isJustReleased()) {
            serial->println("Couleur OFF");
        }

        // GPIO1
        if (pamiHardware->pinGPIO1->isJustPressed()) {
            serial->println("GPIO1 ON");
        } else if (pamiHardware->pinGPIO1->isJustReleased()) {
            serial->println("GPIO1 OFF");
        }

        // GPIO2
        if (pamiHardware->pinGPIO2->isJustPressed()) {
            serial->println("GPIO2 ON");
        } else if (pamiHardware->pinGPIO2->isJustReleased()) {
            serial->println("GPIO2 OFF");
        }
    }

    if (srf08TestEngaged) {
        if (pamiHardware->srf08->checkMeasureResponse()) {
            serial->print("Distance mesuree en cm : ");
            serial->println(pamiHardware->srf08->getLastMeasureCentimeter());
            srf08TestEngaged = srf08TestContinuous;
            if (srf08TestContinuous) {
                testSrf08(true);
            }
        } else if (pamiHardware->srf08->hasTimedOut()) {
            serial->println("timeout du srf08");
            srf08TestEngaged = false;
            srf08TestContinuous = false;
        }
    }

    if (earsContinousTest) {
        if (millis() > earsStartTestTime + 2000) {
            earsStartTestTime = millis();
            if (pamiHardware->servo1->read() < 100) {
                serial->println("oreilles a 180 deg");
                pamiHardware->servo1->write(180);
            } else {
                serial->println("oreilles a 0 deg");
                pamiHardware->servo1->write(0);
            }
        }
    }
}

bool TestManager::isTestingSrf08Continuous() {
    return srf08TestEngaged && srf08TestContinuous;
}

void TestManager::stopSrfTest() {
    srf08TestEngaged = false;
    srf08TestContinuous = false;
}

bool TestManager::toggleEarsTest() {
    earsContinousTest = !earsContinousTest;
    earsStartTestTime = 0;
    return earsContinousTest;
}