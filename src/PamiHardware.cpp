#include "PamiHardware.hpp"

PamiHardWare::PamiHardWare(
    IODebouncer* pinTirette,
    IODebouncer* pinCouleur,
    IODebouncer* pinGPIO1,
    IODebouncer* pinGPIO2
) 
  : pinTirette(pinTirette)
  , pinCouleur(pinCouleur)
  , pinGPIO1(pinGPIO1)
  , pinGPIO2(pinGPIO2)
{
}