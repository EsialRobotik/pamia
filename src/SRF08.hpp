#ifndef _SRF08_HPP
#define _SRF08_HPP

#include <Wire.h>

#define SRF08_COMMAND_RANGING_MODE_INCHES 0x50
#define SRF08_COMMAND_RANGING_MODE_CENTIMETERS 0x51
#define SRF08_COMMAND_RANGING_MODE_MICROSECONDS 0x52

#define SRF08_WRITE_REGISTER_COMMAND 0x00
#define SRF08_WRITE_REGISTER_MAX_GAIN 0x01
#define SRF08_WRITE_REGISTER_RANGE 0x02

#define SRF08_UNCOMPLETED_RANGING_VALUE 0xFF

#define SRF08_1ST_ECHO_HIGH_BYTE_REGISTER 0x02
#define SRF08_1ST_ECHO_LOW_BYTE_REGISTER 0x03

#define SRF08_RANGE_REGISTER_ADDRESS 0x02

class SRF08
{
    public:
        SRF08(int address, unsigned long measureTimeout);
        bool triggerMeasure();
        bool checkMeasureResponse();
        bool hasTimedOut();
        unsigned int getLastMeasureCentimeter();
        void reset();
        int getAddress();
    
    private:
        int address;
        unsigned long lastMeasureTime;
        unsigned long measureTimeout;
        unsigned int lastMeasureCentimeters;
};

#endif