#ifndef _ROBODEM_EROM_H_
#define _ROBODEM_EROM_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Avoid likange bugs in Arduino/AVR libraries
#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#include <avr/eeprom.h>
#undef __AVR_ATmega2560__
#else // __AVR_ATmega2560__
#include <avr/eeprom.h>
#endif // __AVR_ATmega2560__

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#include "erom_Access.h"
#include "erom_Entry.h"
#include "erom_Storage.h"
#include "erom_VerifiedStorage.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

extern erom::Access &access; // Default, ready-to-use, EEPROM access engine

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_H_
