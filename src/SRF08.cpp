#include "SRF08.hpp"
#include <Arduino.h>

SRF08::SRF08(int address, unsigned long measureTimeout)
  : address(address)
  , measureTimeout(measureTimeout)
  , lastMeasureCentimeters(0)
  , lastMeasureTime(0)
{
}

bool SRF08::triggerMeasure() {
    lastMeasureCentimeters = 0;
    lastMeasureTime = millis();
    Wire.beginTransmission(address);
    Wire.write(byte(SRF08_WRITE_REGISTER_COMMAND));
    Wire.write(byte(SRF08_COMMAND_RANGING_MODE_CENTIMETERS));
    return Wire.endTransmission() == 0;
}

bool SRF08::checkMeasureResponse() {
    if (lastMeasureCentimeters != 0) {
        return true;
    }

    Wire.beginTransmission(address);
    Wire.write(byte(SRF08_1ST_ECHO_HIGH_BYTE_REGISTER));
    if (Wire.endTransmission() != 0) {
        return false;
    }

    Wire.requestFrom(address, 2);
    if (Wire.available() < 2) {
        return false;
    }
    int highByte = Wire.read();
    int lowByte = Wire.read();
    
    int range = ((highByte << 8) | lowByte) & 0xFFFF;

    // D'après la doc une valeur de 0xFFFF indique que le srf n'a pas encore terminé la mesure
    if (range == 0xFFFF) {
        return false;
    }
    lastMeasureCentimeters = range;
    return true;
}

bool SRF08::hasTimedOut() {
    return lastMeasureTime != 0 && millis() - lastMeasureTime > measureTimeout;
}

unsigned int SRF08::getLastMeasureCentimeter() {
    return lastMeasureCentimeters;
}

void SRF08::reset() {
    lastMeasureTime = 0;
    lastMeasureCentimeters = 0;
}

int SRF08::getAddress() {
    return address;
}