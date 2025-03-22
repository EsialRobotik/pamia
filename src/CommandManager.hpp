#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include <Arduino.h>
#include <Stream.h>
#include "TestManager.hpp"

// Les commandes disponibles sur la liaison série
enum CommandManagerCommand {
    SERIAL_COMMAND_TEST_TOGGLE = 't',
};

/**
 * @brief Gère les commandes reçues sur une liaison série
 */
class CommandManager
{
    public:
        CommandManager(Stream* serial, TestManager* testManager);

        /**
         * @brief Méthode à appeler régulièrement pour gérer les commandes reçues sur al liaison série
         */
        void heartBeat();

    private:

        /**
         * @brief Analyse et traite les commandes qui arrivent sur la liaison série.
         */
        void handleSerialCommand();
    
        /**
         * @brief Gère la commande moteur PWM du lidar
         */
        void handleMotorControl();

        Stream* serial;
        TestManager* testManager;
};

#endif