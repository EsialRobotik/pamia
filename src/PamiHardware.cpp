#include "PamiHardware.hpp"

PamiHardWare::PamiHardWare(
    IODebouncer* pinTirette,
    IODebouncer* pinCouleur,
    IODebouncer* pinGPIO1,
    IODebouncer* pinGPIO2,
    SRF08* srf08,
    Servo* servo1
) 
  : pinTirette(pinTirette)
  , pinCouleur(pinCouleur)
  , pinGPIO1(pinGPIO1)
  , pinGPIO2(pinGPIO2)
  , srf08(srf08)
  , servo1(servo1)
{
}