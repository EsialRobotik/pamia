#ifndef _TEST_MANAGER_HPP
#define _TEST_MANAGER_HPP

#include <Arduino.h>
#include <Stream.h>
#include "PamiHardware.hpp"

/**
 * @brief Gère le mode test de la PAMI
 */
class TestManager
{
    public:
        TestManager(Stream* serial, PamiHardWare* pamiHardware);

        void heartBeat();

        /**
         * @brief Met en route ou arrête de mode test
         * 
         * @return true Tests démarrés
         * @return false Tests arrêtés
         */
        bool toggle();

    private:
        Stream * serial;
        PamiHardWare* pamiHardware;
        bool on;
};

#endif