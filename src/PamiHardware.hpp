#ifndef _PAMI_HPP
#define _PAMI_HPP

#include "IODebouncer.h"

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
            IODebouncer* pinGPIO2
        );

        IODebouncer* pinTirette;
        IODebouncer* pinCouleur;
        IODebouncer* pinGPIO1;
        IODebouncer* pinGPIO2;
};

#endif