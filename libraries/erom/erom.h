#ifndef _ROBODEM_EROM_H_
#define _ROBODEM_EROM_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Main include file for Arduino. One does not have to include any other
// header files but this one.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Avoid linkage bugs in Arduino/AVR libraries
#ifndef __AVR_ATmega2560__
#define __AVR_ATmega2560__
#include <avr/eeprom.h>
#undef __AVR_ATmega2560__
#else // __AVR_ATmega2560__
#include <avr/eeprom.h>
#endif // __AVR_ATmega2560__

// Arduino stuff
#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Includes all available functionality
#include "erom_Access.h"
#include "erom_Entry.h"
#include "erom_Storage.h"
#include "erom_VerifiedStorage.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Boards with ATmega328, Duemilanove, Uno, UnoSMD, Lilypad:      1024 bytes //
// Boards with ATmega1280 or 2560, Arduino Mega series:           4096 bytes //
// Boards with ATmega168, Lilypad, old Nano, Diecimila:            512 bytes //
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Default, ready-to-use, EEPROM access engine. See 'erom::Access' class for
// more details
extern erom::Access &access;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_H_
