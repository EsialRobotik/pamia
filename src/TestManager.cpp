#include "TestManager.hpp"

TestManager::TestManager(Stream * serial, PamiHardWare* pamiHardware)
    : serial(serial)
    , pamiHardware(pamiHardware)
{
}

bool TestManager::toggle()
{
    on = !on;
    return on;
}

void TestManager::heartBeat()
{
    if (!on) {
        return;
    }

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