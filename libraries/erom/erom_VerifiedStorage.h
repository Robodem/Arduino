#ifndef _ROBODEM_EROM_VERIFIED_STORAGE_H_
#define _ROBODEM_EROM_VERIFIED_STORAGE_H_

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#include "erom_Access.h"
#include "erom_Entry.h"
#include "erom_Storage.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

namespace erom {

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

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
  virtual void OnLoad()  { Storage::OnLoad();   _load_header(); }
  virtual void OnSave()  { Storage::OnSave();   _save_header(); }
  virtual void OnClear() { Storage::OnClear(); _clear_header(); }
  virtual bool OnVerify(uint16_t aAppID, uint16_t aVersion) { return aAppID == app_id() && aVersion == version(); }

public:
  VerifiedStorage(uint16_t aAppID, uint16_t aVersion);
  VerifiedStorage(Access &aAccess, uint16_t aTargetAppID, uint16_t aTargetVersion);

  bool verify(bool aAutoClear = false);

  inline uint16_t app_id()  const { return _app_id;  }
  inline uint16_t version() const { return _version; }

  inline uint16_t stored_app_id()  const { return _stored_app_id.value; }
  inline uint16_t stored_version() const { return _stored_version.value; }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

} // namespace erom

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

#endif // _ROBODEM_EROM_VERIFIED_STORAGE_H_
