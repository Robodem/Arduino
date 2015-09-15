#ifndef _ROBODEM_EROM_ENTRY_H_
#define _ROBODEM_EROM_ENTRY_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif
#include <inttypes.h>
#include "erom_Access.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

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
  type value;

  Entry() { _access = NULL; }
  Entry(size_t aAddress) : _access(&Access::instance()), _address(aAddress) { load(); }
  Entry(size_t aAddress, const type &aValue) : _access(&Access::instance()), _address(aAddress), value(aValue) { /* Do Nothing */ }
  Entry(Access &aAccess, size_t aAddress) : _access(&aAccess), _address(aAddress) { load(); }
  Entry(Access &aAccess, size_t aAddress, const type &aValue) : _access(&aAccess), _address(aAddress), value(aValue) { /* Do Nothing */ }
  Entry(const Entry &O) : _access(O._access), _address(O._address), value(O.value) { /* Do Nothing */ }

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

  // Write from RAM into EEPROM
  // aFullWrite - if true, all data will be written, otherwise changes only
  inline void save(bool aFullWrite = false) const {
    if (_access) {
      if (aFullWrite) _access->write_block(address(), value);
      else _access->update_block(address(), value);
    }
  }

  // Load from EEPROM to RAM
  inline void load() { if (_access) _access->read_block(address(), value); }

  inline size_t address() const { return _address; }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_ENTRY_H_
