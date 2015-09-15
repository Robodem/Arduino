#ifndef _ROBODEM_EROM_STORAGE_H_
#define _ROBODEM_EROM_STORAGE_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#include "erom_Access.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

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
  virtual void OnLoad() { /* Do Nothing */ }
  virtual void OnSave() { /* Do Nothing */ }
  virtual void OnClear() { /* Do Nothing */ }

public:
  Storage() : _access(Access::instance()) { _last_issue = 0; }
  Storage(Access &aAccess) : _access(aAccess) { _last_issue = 0; }
  virtual ~Storage() { /* Do Nothing */ }

  template<typename T> inline Entry<T> issue() {
    Entry<T> __entry(access(), _last_issue);
    _advance_issue(__entry.size);
    return __entry;
  }

  template<typename T> inline Entry<T> issue(const T &aValue) {
    Entry<T> __entry(access(), _last_issue, aValue);
    _advance_issue(sizeof(T));
    return __entry;
  }

  template<typename T> inline Entry<T>& issue(Entry<T> &aEntry) {
    aEntry.set_access(&_access);
    aEntry.set_address(_last_issue);
    _advance_issue(aEntry.size);
    return aEntry;
  }

  inline void load() { OnLoad(); }
  inline void save() { OnSave(); }
  inline void clear(bool aAutoSave = true) { OnClear(); if (aAutoSave) save(); }

  // Postpone save call for the specified delay time (see 'tick()' method).
  //   aDelay - After thich period 'save()' should be called
  //   aRestartDelay - Overrides previously postponed and uncalled save with a new delay
  inline void postpone_save(unsigned long aDelay = DefaultPostponeSaveDelay, bool aRestartDelay = false) {
    if (!_save_requested || aRestartDelay) _save_requested = true, _save_time = millis() + aDelay;
  }

  // Must be called in the main program loop, in order to call 'save()' as per
  // posponed save requests.
  inline void tick() { if (_save_requested && _save_time <= millis()) save(), _save_requested = false; }

  inline Access& access() { return _access; }
  inline const Access& access() const { return _access; }

  inline size_t size() const { return _last_issue; }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_STORAGE_H_
