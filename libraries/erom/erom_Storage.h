#ifndef _ROBODEM_EROM_STORAGE_H_
#define _ROBODEM_EROM_STORAGE_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#include "erom_Access.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// EEPROM storage management, used to issue address locations to 'Entry'
// objects, as well, as initializing, saving and loading multiple entries.
// Also provides postponed save functionality to reduce flash write cycles.
class Storage {
public:
  // Default postponed save delay duration
  static const unsigned long DefaultPostponeSaveDelay = 1000;

private:
  Access &_access;
  size_t _last_issue;
  unsigned long _save_time;
  bool _save_requested;

  inline size_t _advance_issue(size_t aSize) { return _last_issue < access().memory_size() ? _last_issue += aSize : _last_issue; }

protected:
  virtual void OnLoad() { /* Do Nothing */ }  // Override to specify how to load your entries from EEPROM
  virtual void OnSave() { /* Do Nothing */ }  // Override to specify how to save your entries to EEPROM
  virtual void OnClear() { /* Do Nothing */ } // Override to specify how to clear/initialize RAM values with default data

public:
  // Create storage with default access
  Storage() : _access(Access::instance()) { _last_issue = 0; }
  // Create storage with user-defined storage
  Storage(Access &aAccess) : _access(aAccess) { _last_issue = 0; }
  virtual ~Storage() { /* Do Nothing */ }

  // Create an 'Entry' object, issue an address for it and read EEPROM value into RAM
  // Example:
  //  Storage storage;
  //  Entry<int>   v1 = storage.issue<int>();   // v1.address() == 0; read it from EEPROM
  //  Entry<float> v2 = storage.issue<float>(); // v1.address() == 2; read it from EEPROM
  //  ++v1, v2 += 10;                           // Increase 'v1' by 1 and 'v2' by 10
  //  v1.save(), v2.save();                     // Save both values back to EEPROM
  template<typename T> inline Entry<T> issue() {
    Entry<T> __entry(access(), _last_issue);
    _advance_issue(__entry.size);
    return __entry;
  }

  // Creates an 'Entry' and initialized RAM value with aValue
  // Example:
  //  Storage storage;
  //  Entry<int>   v1 = storage.issue<int>(0);     // v1.address() == 0; value == 0
  //  Entry<float> v2 = storage.issue<float>(0.f); // v1.address() == 2; value == 0.f
  //  v1.save(), v2.save();                        // Write values (0 and 0.f) to EEPROM
  template<typename T> inline Entry<T> issue(const T &aValue) {
    Entry<T> __entry(access(), _last_issue, aValue);
    _advance_issue(sizeof(T));
    return __entry;
  }

  // Initialize/reissue/recycle already existing 'Entry' object, issuing it an address
  // Example:
  //  Storage storage;
  //  Entry<int> v1;      // Create default/empty 'Entry' object with unknown RAM value
  //  v1.save();          // Does nothing, as the object cannot be used for EEPROM operations
  //  storage.issue(v1);  // Issue the object an address; object is ready for EEPROM operations
  //  v1.load();          // Read value from EEPROM to RAM
  template<typename T> inline Entry<T>& issue(Entry<T> &aEntry) {
    aEntry.set_access(&_access);
    aEntry.set_address(_last_issue);
    _advance_issue(aEntry.size);
    return aEntry;
  }

  // Loads all values to RAM. The method by itself does nothing but calling
  // the user-defined 'OnLoad()' method. It is up to the user to specify the
  // loading process
  inline void load() { OnLoad(); }
  // Saves all values to EEPROM. The method by itself does nothing but calling
  // the user-defined 'OnSave()' method. It is up to the user to specify the
  // saving process
  inline void save() { OnSave(); }
  // Clears all values to their defaults. If 'aAutoSave' is true, then 'save()'
  // will be called after the clearing is done. The method by itself doesn't
  // nothing but calling the user-defined 'OnClaer()' method and then the 'save()'
  // method is requested. It is up to the user to specify the clearing process
  inline void clear(bool aAutoSave = true) { OnClear(); if (aAutoSave) save(); }

  // Postpone save call for the specified delay time (see 'tick()' method).
  //   aDelay - After which period 'save()' should be called
  //   aRestartDelay - Overrides previously postponed and uncalled save with a new delay
  inline void postpone_save(unsigned long aDelay = DefaultPostponeSaveDelay, bool aRestartDelay = false) {
    if (!_save_requested || aRestartDelay) _save_requested = true, _save_time = millis() + aDelay;
  }

  // Returns true if there is a postponed save pending
  inline bool postpone_save_requested() const { return _save_requested; }

  // Planned save time of a postponed save
  inline unsigned long postpone_save_time() const { return _save_time; }

  // Must be called in the main program loop, in order to call 'save()' as per
  // posponed save requests.
  inline void tick() { if (_save_requested && _save_time <= millis()) save(), _save_requested = false; }

  inline Access& access() { return _access; }
  inline const Access& access() const { return _access; }

  // Returns the amount of bytes issued to entries.
  inline size_t size() const { return _last_issue; }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_STORAGE_H_
