#ifndef _TEST_MANAGER_HPP
#define _TEST_MANAGER_HPP

#include <Arduino.h>
#include <Stream.h>
#include "PamiHardware.hpp"
#include <Wire.h>

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
        bool toggleIoTest();

        /**
         * @brief Lance un test de mesure/lecture de distance sur le SRF08
         * @param continuous si true : le test est effectué en continu
         * 
         */
        void testSrf08(bool continuous);

        /**
         * @brief Indique si un test continu du SRF est en cours
         */
        bool isTestingSrf08Continuous();

        /**
         * @brief Met fin au test du SRF08
         */
        void stopSrfTest();

        /**
         * @brief Active ou désactive le mode test en continu des oreilles
         * @return true si le test est activé, false sinon
         * 
         */
        bool toggleEarsTest();

    private:
        Stream * serial;
        PamiHardWare* pamiHardware;
        bool testIoOn;
        bool srf08TestEngaged;
        bool srf08TestContinuous;
        bool earsContinousTest;
        long earsStartTestTime;
};

#endif