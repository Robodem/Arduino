#ifndef _ROBODEM_EROM_VERIFIED_STORAGE_H_
#define _ROBODEM_EROM_VERIFIED_STORAGE_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#include "erom_Access.h"
#include "erom_Entry.h"
#include "erom_Storage.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// EEPROM storage management, inherits the 'Storage' class and is used to
// verify whether data currently stored in EEPROM is valid and can be used by
// the running application/sketch.
class VerifiedStorage : public Storage {
public:
  static const uint32_t storage_header_value = 0xDEADBEEF;

private:
  const uint16_t _app_id, _version;

  Entry<uint32_t> _header;
  Entry<uint16_t> _stored_app_id, _stored_version;

  inline void _load_header() { _header.load(); _stored_app_id.load(); _stored_version.load(); }
  inline void _save_header() { _header.save(); _stored_app_id.save(); _stored_version.save(); }
  inline void _clear_header() { _header = storage_header_value; _stored_app_id  = app_id(); _stored_version = version(); }

protected:
  // Override to specify how to load your entries from EEPROM.
  // NOTE: when overridden, 'VerifiedStorage::OnLoad()' must be called by user
  // in order for 'VerifiedStorage' to work properly
  virtual void OnLoad()  { Storage::OnLoad();   _load_header(); }

  // Override to specify how to save your entries to EEPROM.
  // NOTE: when overridden, 'VerifiedStorage::OnSave()' must be called by user
  // in order for 'VerifiedStorage' to work properly
  virtual void OnSave()  { Storage::OnSave();   _save_header(); }

  // Override to specify how to clear/initialize RAM values with default data.
  // NOTE: when overridden, 'VerifiedStorage::OnClear()' must be called by user
  // in order for 'VerifiedStorage' to work properly
  virtual void OnClear() { Storage::OnClear(); _clear_header(); }

  // Override to specify the checking rules for data validity. By default
  // EEPROM data AppID and VersionID must be equal to sketch's AppID and Version
  // NOTE: when overridden, 'VerifiedStorage::OnVerify()' must be called by user
  // in order for 'VerifiedStorage' to work properly
  virtual bool OnVerify(uint16_t aAppID, uint16_t aVersion) { return aAppID == app_id() && aVersion == version(); }

public:
  // Create 'VerifiedStorage' with specified Application ID and Version Number
  // for use with default access.
  VerifiedStorage(uint16_t aAppID, uint16_t aVersion);

  // Create 'VerifiedStorage' with specified Application ID and Version Number
  // for use with user-defined access.
  VerifiedStorage(Access &aAccess, uint16_t aTargetAppID, uint16_t aTargetVersion);

  // Verify storage version (AppID and VersionNo). If 'aAutoClear', 'verify'
  // will call 'clear' method, in order to reinitialize variables in RAM and
  // stored cleared RAM values to EEPROM.
  // Returns true, if EEPROM data suits currently running application/sketch
  // verification rules.
  // To define verification rules, one must override the 'OnVerify()' method.
  bool verify(bool aAutoClear = false);

  // Currently running application/sketch AppID and VersionNo
  inline uint16_t app_id()  const { return _app_id;  }
  inline uint16_t version() const { return _version; }

  // AppID and VersionNo stored in EEPROM
  inline uint16_t stored_app_id()  const { return _stored_app_id.value; }
  inline uint16_t stored_version() const { return _stored_version.value; }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_VERIFIED_STORAGE_H_
