#ifndef _ROBODEM_EROM_ACCESS_H_
#define _ROBODEM_EROM_ACCESS_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif
#include <inttypes.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Boards with ATmega328, Duemilanove, Uno, UnoSMD, Lilypad:      1024 bytes //
// Boards with ATmega1280 or 2560, Arduino Mega series:           4096 bytes //
// Boards with ATmega168, Lilypad, old Nano, Diecimila:            512 bytes //
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

class Access {
private:
  size_t _base, _memory_size;

  template<class T> inline void  _read_block(size_t aAddress, const T &aValue) const {
    eeprom_read_block((void*)&aValue, (const void*)aAddress, sizeof(aValue));
  }

  template<class T> inline void _write_block(size_t aAddress, const T &aValue) const {
    eeprom_write_block(reinterpret_cast<const void*>(&aValue), reinterpret_cast<void*>(aAddress), sizeof(aValue));
  }

public:
  Access(size_t aBase = 0);

  inline uint8_t  read       (size_t aAddress) const { return read_byte(aAddress); }
         bool     read_bit   (size_t aAddress, uint8_t aBit) const ;
  inline uint8_t  read_byte  (size_t aAddress) const { return in_range(aAddress + sizeof(uint8_t))  ? eeprom_read_byte ((unsigned char*) aAddress) : 0; }
  inline char     read_char  (size_t aAddress) const { return (char)read_byte(aAddress); }
  inline uint16_t read_int   (size_t aAddress) const { return in_range(aAddress + sizeof(uint16_t)) ? eeprom_read_word ((uint16_t*)aAddress)       : 0; }
  inline uint32_t read_long  (size_t aAddress) const { return in_range(aAddress + sizeof(uint32_t)) ? eeprom_read_dword((unsigned long*)aAddress)  : 0; }
         float    read_float (size_t aAddress) const;
         double   read_double(size_t aAddress) const;

  inline bool write       (size_t aAddress, uint8_t  aValue) const { return write_byte(aAddress, aValue); }
         bool write_bit   (size_t aAddress, uint8_t  aBit, bool aValue) const { return update_bit(aAddress, aBit, aValue); }
  inline bool write_byte  (size_t aAddress, uint8_t  aValue) const { return in_range(aAddress + sizeof(uint8_t))  ? eeprom_write_byte((unsigned char*)aAddress, aValue), true   : false; }
  inline bool write_char  (size_t aAddress, char     aValue) const { return write_byte(aAddress, (uint8_t)aValue); }
  inline bool write_int   (size_t aAddress, uint16_t aValue) const { return in_range(aAddress + sizeof(uint16_t)) ? eeprom_write_word((uint16_t*)aAddress, aValue), true        : false; }
  inline bool write_long  (size_t aAddress, uint32_t aValue) const { return in_range(aAddress + sizeof(uint32_t)) ? eeprom_write_dword((unsigned long*) aAddress, aValue), true : false; }
  inline bool write_float (size_t aAddress, float    aValue) const { return write_block<float>(aAddress, aValue) != 0; }
  inline bool write_double(size_t aAddress, double   aValue) const { return write_block<double>(aAddress, aValue) != 0; }

  inline bool update       (size_t aAddress, uint8_t  aValue) const { return update_byte(aAddress, aValue); }
         bool update_bit   (size_t aAddress, uint8_t  aBit, bool aValue) const;
  inline bool update_byte  (size_t aAddress, uint8_t  aValue) const { return update_block<uint8_t>(aAddress, aValue) != 0; }
  inline bool update_char  (size_t aAddress, char     aValue) const { return update_block<char>(aAddress, aValue) != 0; }
  inline bool update_int   (size_t aAddress, uint16_t aValue) const { return update_block<uint16_t>(aAddress, aValue) != 0; }
  inline bool update_long  (size_t aAddress, uint32_t aValue) const { return update_block<uint32_t>(aAddress, aValue) != 0; }
  inline bool update_float (size_t aAddress, float aValue)    const { return update_block<float>(aAddress, aValue) != 0; }
  inline bool update_double(size_t aAddress, double aValue)   const { return write_block<double>(aAddress, aValue) != 0; }
  

  // Use template for other data formats
  template<class T> inline bool read_block(size_t aAddress, const T& aValue) const {
    if (!in_range(aAddress + sizeof(T))) return false;
    else { _read_block(aAddress, aValue); return true; }
  }

  template<class T> size_t read_block(size_t aAddress, const T aValue[], int aItems) const {
    if (!in_range(aAddress + aItems * sizeof(T))) return 0;
    for (size_t __i = 0; __i < aItems; __i++) _read_block<T>(aAddress + __i * sizeof(T), aValue[__i]);
    return aItems;
  }

  template<class T> inline bool write_block(size_t aAddress, const T& aValue) const {
    if (!in_range(aAddress + sizeof(aValue))) return false;
    else { _write_block(aAddress, aValue); return true; }
  }

  template<class T> size_t write_block(size_t aAddress, const T aValue[], size_t aItems) const {
    if (!in_range(aAddress + aItems * sizeof(T))) return 0;
    for (size_t __i = 0; __i < aItems; __i++) _write_block<T>(aAddress + __i * sizeof(T), aValue[__i]);
    return aItems;
  }

  template<class T> int update_block(size_t aAddress, const T aValue[], size_t aItems) const {
    if (!in_range(aAddress + aItems * sizeof(T))) return 0;
    for (size_t __i = 0; __i < aItems; __i++)
      update_block<T>(aAddress + (__i * sizeof(T)), aValue[__i]);
    return aItems;
  }
  
  template<class T> size_t update_block(size_t aAddress, const T &aValue) const {
    if (!in_range(aAddress + sizeof(aValue))) return 0;
    const byte *__p = (const byte*)(const void*)&aValue;
    size_t __written = 0;

    for (size_t __i = 0; __i < sizeof(aValue); __i++) {
      if (read(aAddress) != *__p) write(aAddress, *__p), __written++;    
      aAddress++, __p++;
    }
    return __written;
  }

  inline bool is_ready() const { return eeprom_is_ready(); }
  inline bool in_range(size_t aAddress) const { return aAddress + base() < memory_size(); }

  inline size_t base() const { return _base; }
  inline size_t base(size_t aBase) { return _base = aBase; }

  inline size_t memory_size() const { return _memory_size; }
  inline void   memory_size(size_t aMemorySize) { _memory_size = aMemorySize; }

public:
  static size_t device_memory_size(); // { return 512; }
  static Access& instance() { static Access access; return access; }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_ACCESS_H_
