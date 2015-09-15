#include "erom.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

erom::Access &access = Access::instance();

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

Access::Access(size_t aBase) : _base(aBase), _memory_size(device_memory_size()) { /* Do Nothing */ }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

bool Access::read_bit(size_t aAddress, byte aBit) const {
  if (aBit > 7 || !in_range(aAddress + sizeof(uint8_t))) return false; 
  byte byteVal = eeprom_read_byte((unsigned char *) aAddress);      
  return (byteVal & (1 << aBit));
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

float Access::read_float(size_t aAddress) const {
  if (!in_range(aAddress + sizeof(float))) return 0.f;
  float __value;
  return read_block<float>(aAddress, __value) ? __value : 0.f;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

double Access::read_double(size_t aAddress) const {
  if (!in_range(aAddress + sizeof(double))) return 0.;
  double __value;
  return read_block<double>(aAddress, __value) ? __value : 0.;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

bool Access::update_bit(size_t aAddress, uint8_t aBit, bool aValue) const {
    if (aBit > 7 || !in_range(aAddress)) return false; 
    
    byte __input  = read_byte(aAddress), __output = __input;    
    if (aValue) __output |= (1 << aBit);
    else __output &= ~(1 << aBit);

    return __output != __input ? write_byte(aAddress, __output) : true;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

size_t Access::device_memory_size() {
  return
#if   defined (__AVR_AT94K__)         \
   || defined (__AVR_AT76C711__)      \
   || defined (__AVR_AT43USB320__)    \
   || defined (__AVR_AT43USB355__)
                                            0         // No EEPROM Memory
#elif defined (__AVR_ATtiny13__)      \
   || defined (__AVR_ATtiny13A__)     \
   || defined (__AVR_ATtiny43U__)     \
   || defined (__AVR_ATtiny48__)      \
   || defined (__AVR_ATtiny88__)
                                            64        // 64 bytes
#elif defined (__AVR_ATtiny22__)      \
   || defined (__AVR_ATtiny26__)      \
   || defined (__AVR_AT90S2343__)     \
   || defined (__AVR_AT90S2333__)     \
   || defined (__AVR_AT90S2323__)     \
   || defined (__AVR_AT90S2313__)     \
   || defined (__AVR_ATtiny2313__)    \
   || defined (__AVR_ATtiny24__)      \
   || defined (__AVR_ATtiny25__)      \
   || defined (__AVR_ATtiny261__)
                                            128       // 128 bytes
#elif defined (__AVR_ATmega48__)      \
   || defined (__AVR_ATmega48P__)     \
   || defined (__AVR_ATmega8HVA__)    \
   || defined (__AVR_AT90S4414__)     \
   || defined (__AVR_AT90S4434__)     \
   || defined (__AVR_AT90S4433__)     \
   || defined (__AVR_ATtiny44__)      \
   || defined (__AVR_ATtiny45__)      \
   || defined (__AVR_ATtiny461__)
                                            256       // 256 bytes
#elif defined (__AVR_ATmega164P__)    \
   || defined (__AVR_ATmega16__)      \
   || defined (__AVR_ATmega161__)     \
   || defined (__AVR_ATmega162__)     \
   || defined (__AVR_ATmega163__)     \
   || defined (__AVR_ATmega165__)     \
   || defined (__AVR_ATmega165P__)    \
   || defined (__AVR_ATmega168__)     \
   || defined (__AVR_ATmega168P__)    \
   || defined (__AVR_ATmega169__)     \
   || defined (__AVR_ATmega169P__)    \
   || defined (__AVR_ATmega16HVA__)   \
   || defined (__AVR_ATmega406__)     \
   || defined (__AVR_ATmega8__)       \
   || defined (__AVR_ATmega8U2__)     \
   || defined (__AVR_ATmega88__)      \
   || defined (__AVR_ATmega88P__)     \
   || defined (__AVR_ATmega8515__)    \
   || defined (__AVR_ATmega8535__)    \
                                      \
   || defined (__AVR_ATtiny167__)     \
   || defined (__AVR_ATtiny84__)      \
   || defined (__AVR_ATtiny85__)      \
   || defined (__AVR_ATtiny861__)     \
                                      \
   || defined (__AVR_AT90S8515__)     \
   || defined (__AVR_AT90C8534__)     \
   || defined (__AVR_AT90S8535__)     \
   || defined (__AVR_AT90PWM1__)      \
   || defined (__AVR_AT90PWM2__)      \
   || defined (__AVR_AT90PWM2B__)     \
   || defined (__AVR_AT90PWM2B__)     \
   || defined (__AVR_AT90PWM3__)      \
   || defined (__AVR_AT90PWM3B__)     \
   || defined (__AVR_AT90PWM216__)    \
   || defined (__AVR_AT90PWM316__)    \
   || defined (__AVR_AT90USB82__)     \
   || defined (__AVR_AT90USB162__)
                                            512       // 512 bytes
#elif defined (__AVR_ATmega32C1__)    \
   || defined (__AVR_ATmega32M1__)    \
   || defined (__AVR_ATmega32U4__)    \
   || defined (__AVR_ATmega32U6__)    \
   || defined (__AVR_AT90CAN32__)     \
   || defined (__AVR_ATmega32__)      \
   || defined (__AVR_ATmega323__)     \
   || defined (__AVR_ATmega325__)     \
   || defined (__AVR_ATmega325P__)    \
   || defined (__AVR_ATmega3250__)    \
   || defined (__AVR_ATmega3250P__)   \
   || defined (__AVR_ATmega329__)     \
   || defined (__AVR_ATmega329P__)    \
   || defined (__AVR_ATmega3290__)    \
   || defined (__AVR_ATmega3290P__)   \
   || defined (__AVR_ATmega324P__)    \
   || defined (__AVR_ATmega328P__)    \
   || defined (__AVR_ATmega32HVB__)
                                            1024      // 1 KB
#elif defined (__AVR_AT90CAN64__)     \
   || defined (__AVR_AT90USB646__)    \
   || defined (__AVR_AT90USB647__)    \
   || defined (__AVR_ATmega64__)      \
   || defined (__AVR_ATmega644__)     \
   || defined (__AVR_ATmega644P__)    \
   || defined (__AVR_ATmega645__)     \
   || defined (__AVR_ATmega6450__)    \
   || defined (__AVR_ATmega649__)     \
   || defined (__AVR_ATmega6490__)
                                            2048      // 2 KB
#elif defined (__AVR_ATmega128__)     \
   || defined (__AVR_ATmega1280__)    \
   || defined (__AVR_ATmega1281__)    \
   || defined (__AVR_ATmega1284P__)   \
   || defined (__AVR_ATmega128RFA1__) \
   || defined (__AVR_ATmega2560__)    \
   || defined (__AVR_ATmega2561__)    \
   || defined (__AVR_AT90CAN128__)    \
   || defined (__AVR_AT90USB1286__)   \
   || defined (__AVR_AT90USB1287__)   \
   || defined (__AVR_ATmega640__)     \
   || defined (__AVR_ATmega103__)
                                            4096      // 4 KB
#elif defined (__AVR_AT86RF401__)
                                            131072    // 128 KB
#else
                                            0         // Memory unknown, specify manually
#endif 
  ;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

VerifiedStorage::VerifiedStorage(uint16_t aAppID, uint16_t aVersion) :
  Storage(),
  _app_id(aAppID), _version(aVersion),
  _header(issue<uint32_t>()),
  _stored_app_id(issue<uint16_t>()),
  _stored_version(issue<uint16_t>())
{
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

VerifiedStorage::VerifiedStorage(Access &aAccess, uint16_t aAppID, uint16_t aVersion) :
  Storage(aAccess),
  _app_id(aAppID), _version(aVersion),
  _header(issue<uint32_t>()),
  _stored_app_id(issue<uint16_t>()),
  _stored_version(issue<uint16_t>())
{
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

bool VerifiedStorage::verify(bool aAutoClear) {
  _load_header();
  bool __ok = _header == storage_header_value && OnVerify(stored_app_id(), stored_version());
  if (!__ok && aAutoClear) { clear(); return verify(false); }
  return __ok;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
