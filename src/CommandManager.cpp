#include "commandmanager.hpp"

CommandManager::CommandManager(Stream* serial, TestManager* testManager)
  : serial(serial)
  , testManager(testManager)
{
}

void CommandManager::handleSerialCommand() {
  if (serial->available() == 0) {
    return;
  }
  delay(5); // on laisse le temps au reste de la commande d'arriver

  char cmd = serial->read();

  switch (cmd) {
    case CommandManagerCommand::SERIAL_COMMAND_TEST_TOGGLE:
      serial->println(testManager->toggle() ? "Test ON" : "Tests OFF");
    break;
  }
}

void CommandManager::heartBeat() {
    handleSerialCommand();
}