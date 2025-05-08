#ifndef _PAMI_HPP
#define _PAMI_HPP

#include "IODebouncer.h"
#include "SRF08.hpp"
#include <ESP32Servo.h>

/**
 * @brief Centralise les accès au hardware de la PAMI
 * 
 */
class PamiHardWare
{
    public:
        PamiHardWare(
            IODebouncer* pinTirette,
            IODebouncer* pinCouleur,
            IODebouncer* pinGPIO1,
            IODebouncer* pinGPIO2,
            SRF08* srf08,
            Servo* servo1
        );

        IODebouncer* pinTirette;
        IODebouncer* pinCouleur;
        IODebouncer* pinGPIO1;
        IODebouncer* pinGPIO2;
        SRF08* srf08;
        Servo* servo1;
};

#endif