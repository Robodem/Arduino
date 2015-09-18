#ifndef _ROBODEM_EROM_ENTRY_H_
#define _ROBODEM_EROM_ENTRY_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#include "erom_Access.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// A template class that provides easy transition from EEPROM storage to RAM
// and vice-versa.
template<typename T> class Entry {
friend class Storage;

public:
  typedef T type;
  enum { size = sizeof(type) };

private:
  Access *_access;
  size_t _address;

protected:
  inline void set_access(Access *aAccess) { _access = aAccess; }
  inline void set_address(size_t aAddress) { _address = aAddress; }

public:
  type value;   // Data stored in RAM

  // Create a null referenced entry. It wont' be able to interact with EEPROM.
  // Used in 'Storage'.
  Entry() { _access = NULL; }
  // Create a referenced entry with default access (Access::instance()) with
  // manually defined address. Initializes RAM value with with the one in EEPROM.
  Entry(size_t aAddress) : _access(&Access::instance()), _address(aAddress) { load(); }
  // Create a referenced entry with default access (Access::instance()) with
  // manually defined address. Initialized RAM value with aValue.
  Entry(size_t aAddress, const type &aValue) : _access(&Access::instance()), _address(aAddress), value(aValue) { /* Do Nothing */ }
  // Create a referenced entry with given access and manually defined address.
  // Initializes RAM value with with the one in EEPROM.
  Entry(Access &aAccess, size_t aAddress) : _access(&aAccess), _address(aAddress) { load(); }
  // Create a referenced entry with given access and manually defined address
  // and initializes RAM value with with aValue.
  Entry(Access &aAccess, size_t aAddress, const type &aValue) : _access(&aAccess), _address(aAddress), value(aValue) { /* Do Nothing */ }
  Entry(const Entry &O) : _access(O._access), _address(O._address), value(O.value) { /* Do Nothing */ }

  // All kinds of operator functionality
  // Example1:
  //  Entry<int> v(0);      // Loads value from EEPROM automatically using default access
  //  int old_v = v;        // Type cast operator example
  //  ++v;                  // Increase loaded value from EEPROM
  //  v.save();             // Store increased value to EEPROM
  //  Serial.print("Old value: "); Serial.print(old_v);
  //  Serial.print("; New value: ");
  //  Serial.print(v.value);  // Ensure 'print' works with original type (i.e., 'int')
  //
  // Example2:
  //  Entry<int> v(0, 100); // Set EEPROM address to '0' and RAM value to '100'
  //  v.save();             // Write value ('100') to EEPROM
  //  v = 0;                // Set RAM value to '0'
  //  v.load();             // Load value from EEPROM
  //  if (v != 100) Serial.print("EEPROM write error!");
  inline operator type() const { return value; }

  inline Entry& operator=(const type &aValue) { value = aValue; return *this; }
  inline type& operator+=(const type &aValue) { return value += aValue; }
  inline type& operator-=(const type &aValue) { return value -= aValue; }	 	  

  inline type& operator++() { ++value; return value; }
  inline type  operator++(int) { type __v(value); ++value; return __v; }

  inline type& operator--() { --value; return *this; }
  inline type  operator--(int) { type __v(value); --value; return __v; }

  inline bool operator <(const type &aValue) const { return value  < aValue; }
  inline bool operator >(const type &aValue) const { return value  > aValue; }
  inline bool operator<=(const type &aValue) const { return value <= aValue; }
  inline bool operator>=(const type &aValue) const { return value >= aValue; }
  inline bool operator==(const type &aValue) const { return value == aValue; }
  inline bool operator!=(const type &aValue) const { return value != aValue; }

  template<typename OT> inline type& operator+=(const OT &aValue) { return value += aValue; }
  template<typename OT> inline type& operator-=(const OT &aValue) { return value -= aValue; }
  template<typename OT> inline type& operator/=(const OT &aValue) { return value /= aValue; }
  template<typename OT> inline type& operator*=(const OT &aValue) { return value *= aValue; }

  template<typename OT> inline bool  operator <(const OT &aValue) const { return value  < aValue; }
  template<typename OT> inline bool  operator >(const OT &aValue) const { return value  > aValue; }
  template<typename OT> inline bool  operator<=(const OT &aValue) const { return value <= aValue; }
  template<typename OT> inline bool  operator>=(const OT &aValue) const { return value >= aValue; }
  template<typename OT> inline bool  operator==(const OT &aValue) const { return value == aValue; }
  template<typename OT> inline bool  operator!=(const OT &aValue) const { return value != aValue; }

  inline Entry& assign(const type &aValue) { value = aValue; return *this; }

  // Write RAM value into EEPROM
  // aFullWrite - if true, all data will be written, otherwise changes only
  inline void save(bool aFullWrite = false) const {
    if (_access) {
      if (aFullWrite) _access->write_block(address(), value);
      else _access->update_block(address(), value);
    }
  }

  // Load value from EEPROM to RAM
  inline void load() { if (_access) _access->read_block(address(), value); }

  // Address of value in EEPROM storage
  inline size_t address() const { return _address; }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_ENTRY_H_
