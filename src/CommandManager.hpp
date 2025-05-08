#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include <Arduino.h>
#include <Stream.h>
#include "TestManager.hpp"

// Les commandes disponibles sur la liaison série
enum CommandManagerCommand {
    SERIAL_COMMAND_TOGGLE_SRF_CONTINUOUS_TEST = 'c',
    SERIAL_COMMAND_TOGGLE_EARS_CONTINUOUS_TEST = 'e',
    SERIAL_COMMAND_SCAN_I2C = 'i',
    SERIAL_COMMAND_TEST_SRF_SINGLE_SHOT = 's',
    SERIAL_COMMAND_TOGGLE_IO_TEST = 't',
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

        Stream* serial;
        TestManager* testManager;
};

#endif