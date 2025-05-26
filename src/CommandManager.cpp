#include "CommandManager.hpp"

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
    case CommandManagerCommand::SERIAL_COMMAND_TOGGLE_IO_TEST:
      serial->println(testManager->toggleIoTest() ? "Test ON" : "Tests OFF");
      break;
    case CommandManagerCommand::SERIAL_COMMAND_TEST_SRF_SINGLE_SHOT:
      testManager->testSrf08(false);
      break;
    case CommandManagerCommand::SERIAL_COMMAND_TOGGLE_SRF_CONTINUOUS_TEST:
      if (testManager->isTestingSrf08Continuous()) {
        testManager->stopSrfTest();
      } else {
        testManager->testSrf08(true);
      }
      break;
    case CommandManagerCommand::SERIAL_COMMAND_TOGGLE_EARS_CONTINUOUS_TEST:
      if (testManager->toggleEarsTest()) {
        serial->println("Test oreilles ON");
      } else {
        serial->println("Test oreilles OFF");
      }
      break;
    case CommandManagerCommand::SERIAL_COMMAND_SCAN_I2C:
      for (int i=0; i<255; i++) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {
          Serial.print("responding i2c addr : ");
          Serial.println(i);
        }
      }
      break;
  }
}

void CommandManager::heartBeat() {
    handleSerialCommand();
}