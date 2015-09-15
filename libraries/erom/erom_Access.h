#ifndef _ROBODEM_EROM_ACCESS_H_
#define _ROBODEM_EROM_ACCESS_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#include <inttypes.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Provides basic access (read/write) to EEPROM. Works with basic types,
// strings and structs.
class Access {
private:
  size_t _base, _memory_size;

  template<class T> inline void  _read_block(size_t aAddress, T &aValue) const {
    eeprom_read_block((void*)&aValue, reinterpret_cast<const void*>(aAddress + base()), sizeof(aValue));
  }

  template<class T> inline void _write_block(size_t aAddress, const T &aValue) const {
    eeprom_write_block(reinterpret_cast<const void*>(&aValue), reinterpret_cast<void*>(aAddress + base()), sizeof(aValue));
  }

public:
  // Constructor
  // Use aBase to specify offset of the zero address.
  // Example:
  //   erom::Access access(32);
  //   access.write(4, 0x1F); // Write 0x1F to EEPROM at address 36
  Access(size_t aBase = 0);

  /////////////////////////////////////////////////////////////////////////////
  // Read data from EEPROM methods set. Return read data or 0 if failed
  inline uint8_t  read       (size_t aAddress) const { return read_byte(aAddress); }
         bool     read_bit   (size_t aAddress, uint8_t aBit) const;
  inline uint8_t  read_byte  (size_t aAddress) const { return in_range(aAddress + sizeof(uint8_t))  ? eeprom_read_byte (reinterpret_cast<unsigned char*>(aAddress + base())) : 0; }
  inline char     read_char  (size_t aAddress) const { return static_cast<char>(read_byte(aAddress)); }
  inline uint16_t read_int   (size_t aAddress) const { return in_range(aAddress + sizeof(uint16_t)) ? eeprom_read_word (reinterpret_cast<uint16_t*>(aAddress + base()))      : 0; }
  inline uint32_t read_long  (size_t aAddress) const { return in_range(aAddress + sizeof(uint32_t)) ? eeprom_read_dword(reinterpret_cast<unsigned long*>(aAddress + base())) : 0; }
         float    read_float (size_t aAddress) const; // Returns 0.f if failed
         double   read_double(size_t aAddress) const; // Returns 0.  if failed

  /////////////////////////////////////////////////////////////////////////////
  // Write data methods set. Returns true if succeeded.
  inline bool write       (size_t aAddress, uint8_t  aValue) const { return write_byte(aAddress, aValue); }
         bool write_bit   (size_t aAddress, uint8_t  aBit, bool aValue) const { return update_bit(aAddress, aBit, aValue); }
  inline bool write_byte  (size_t aAddress, uint8_t  aValue) const { return in_range(aAddress + sizeof(uint8_t))  ? eeprom_write_byte(reinterpret_cast<unsigned char*>(aAddress + base()), aValue), true : false; }
  inline bool write_char  (size_t aAddress, char     aValue) const { return write_byte(aAddress, (uint8_t)aValue); }
  inline bool write_int   (size_t aAddress, uint16_t aValue) const { return in_range(aAddress + sizeof(uint16_t)) ? eeprom_write_word(reinterpret_cast<uint16_t*>(aAddress + base()), aValue),      true : false; }
  inline bool write_long  (size_t aAddress, uint32_t aValue) const { return in_range(aAddress + sizeof(uint32_t)) ? eeprom_write_dword(reinterpret_cast<unsigned long*>(aAddress + base()), aValue),true : false; }
  inline bool write_float (size_t aAddress, float    aValue) const { return write_block<float>(aAddress, aValue) != 0; }
  inline bool write_double(size_t aAddress, double   aValue) const { return write_block<double>(aAddress, aValue) != 0; }

  /////////////////////////////////////////////////////////////////////////////
  // Update data methods set (write only bytes that differ from EEPROM).
  // Returns true if any changes took place.
  inline bool update       (size_t aAddress, uint8_t  aValue) const { return update_byte(aAddress, aValue); }
         bool update_bit   (size_t aAddress, uint8_t  aBit, bool aValue) const;
  inline bool update_byte  (size_t aAddress, uint8_t  aValue) const { return update_block<uint8_t>(aAddress, aValue) != 0; }
  inline bool update_char  (size_t aAddress, char     aValue) const { return update_block<char>(aAddress, aValue) != 0; }
  inline bool update_int   (size_t aAddress, uint16_t aValue) const { return update_block<uint16_t>(aAddress, aValue) != 0; }
  inline bool update_long  (size_t aAddress, uint32_t aValue) const { return update_block<uint32_t>(aAddress, aValue) != 0; }
  inline bool update_float (size_t aAddress, float aValue)    const { return update_block<float>(aAddress, aValue) != 0; }
  inline bool update_double(size_t aAddress, double aValue)   const { return write_block<double>(aAddress, aValue) != 0; }

  /////////////////////////////////////////////////////////////////////////////
  // Templates for complex data types
  /////////////////////////////////////////////////////////////////////////////
  // Read user-defined type from EEPROM
  // Returns true upon success
  // Example:
  //  struct data_t { int a, b; };
  //  data_t data;
  //  erom::access.read_block(0, data);
  template<class T> inline bool read_block(size_t aAddress, T& aValue) const {
    if (!in_range(aAddress + sizeof(T))) return false;
    else { _read_block(aAddress, aValue); return true; }
  }

  // Read an array from EEPROM
  // Returns true upon success
  // Example:
  //  int data[4];
  //  erom::access.read_block(0, data, 10);
  template<class T> size_t read_block(size_t aAddress, T aValue[], int aItems) const {
    if (!in_range(aAddress + aItems * sizeof(T))) return 0;
    for (size_t __i = 0; __i < aItems; __i++) _read_block<T>(aAddress + __i * sizeof(T), aValue[__i]);
    return aItems;
  }

  // Write user-defined type to EEPROM
  // Returns true upon success
  // Example:
  //  struct data_t { int a, b; };
  //  data_t data; data.a = 10; data.b = 20;
  //  erom::access.write_block(0, data);
  template<class T> inline bool write_block(size_t aAddress, const T& aValue) const {
    if (!in_range(aAddress + sizeof(aValue))) return false;
    else { _write_block(aAddress, aValue); return true; }
  }

  // Write an array to EEPROM
  // Returns true upon success
  // Example:
  //  int data[4] = { 0, 1, 2, 3};
  //  erom::access.write_block(0, data, 4);
  template<class T> size_t write_block(size_t aAddress, const T aValue[], size_t aItems) const {
    if (!in_range(aAddress + aItems * sizeof(T))) return 0;
    for (size_t __i = 0; __i < aItems; __i++) _write_block<T>(aAddress + __i * sizeof(T), aValue[__i]);
    return aItems;
  }

  // Write user-defined type to EEPROM (changes only)
  // Returns number of bytes written to EEPROM
  // Example:
  //  struct data_t { int a, b; };
  //  data_t data; data.a = 10; data.b = 20;
  //  erom::access.write_block(0, data); // write whole 'data' to EEPROM
  //  data.a = 30;
  //  erom::access.update_block(0, data); // write 'data.a' only to EEPROM
  template<class T> size_t update_block(size_t aAddress, const T &aValue) const {
    if (!in_range(aAddress + sizeof(aValue))) return 0;
    const uint8_t *__p = reinterpret_cast<const uint8_t*>(&aValue);
    size_t __written = 0;

    for (size_t __i = 0; __i < sizeof(aValue); __i++) {
      if (read(aAddress) != *__p) write(aAddress, *__p), __written++;    
      aAddress++, __p++;
    }
    return __written;
  }

  // Write an array to EEPROM (changes only)
  // Returns number of bytes written to EEPROM
  // Example:
  //  int data[4] = { 0, 1, 2, 3};
  //  erom::access.write_block(0, data, 4);
  //  data[0] = 5; data[3] = 6;
  //  erom::access.update_block(0, data, 4); // write 'data[0]' and 'data[3]' only
  template<class T> size_t update_block(size_t aAddress, const T aValue[], size_t aItems) const {
    if (!in_range(aAddress + aItems * sizeof(T))) return 0;
    for (size_t __i = 0; __i < aItems; __i++)
      update_block<T>(aAddress + (__i * sizeof(T)), aValue[__i]);
    return aItems;
  }

  // Returns true if EEPROM is ready for work
  inline bool is_ready() const { return eeprom_is_ready(); }
  // Returns true if given address fits the range 'base()' .. 'memory_size()'
  inline bool in_range(size_t aAddress) const { return aAddress + base() < memory_size(); }

  // Starting logical point of EEPROM access
  inline size_t base() const { return _base; }
  inline size_t base(size_t aBase) { return _base = aBase; }

  // EEPROM memory size, can be set by user manually, set to chip's actual size by default CPU's 
  inline size_t memory_size() const { return _memory_size; }
  inline void   memory_size(size_t aMemorySize) { _memory_size = aMemorySize; }

public:
  // Chip's EEPROM actual size
  static size_t device_memory_size();
  // Ready-to-use access instance
  static Access& instance() { static Access access; return access; }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_ACCESS_H_
