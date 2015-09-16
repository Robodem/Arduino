#include <Arduino.h>
#include <erom.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

class Storage : public erom::VerifiedStorage {
protected:
  virtual void OnClear() {
    erom::VerifiedStorage::OnClear(); // Must be called for verification to work
    uptime = 0; serial_bytes_in = 0;
  }

  virtual void OnLoad() {
    erom::VerifiedStorage::OnLoad();  // Must be called for verification to work
    uptime.load(); serial_bytes_in.load();
  }

  virtual void OnSave() {
    erom::VerifiedStorage::OnSave();  // Must be called for verification to work

    Serial.println("Saving data to EEPROM...");
    uptime.save(); serial_bytes_in.save();
  }

public:
  erom::Entry<long> uptime;
  erom::Entry<long> serial_bytes_in;

  Storage() : VerifiedStorage(0xFFF1, 0x0001) { issue(uptime); issue(serial_bytes_in); }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

Storage storage;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

unsigned long update_data_time = 0;
unsigned long last_update_time = 0;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

inline void print_values() {
  Serial.print("Total uptime: ");
  Serial.print(storage.uptime.value / 1000);
  Serial.println("s");
  Serial.print("Serial bytes in: ");
  Serial.println(storage.serial_bytes_in.value);
  Serial.println();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void serialEvent() {
  while (Serial.available() > 0) {
    ++storage.serial_bytes_in;
    Serial.read();
  }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void setup() {
  Serial.begin(115200);

  if (!storage.verify(true)) {
    Serial.println("Cannot initialize EEPROM. STOPPED!");
    while (1);
  }

  delay(1000);

  Serial.println("---------------------");
  Serial.print("EEPROM Size: "); Serial.println(erom::Access::device_memory_size());
  Serial.print("Storage size: ");
  Serial.println(storage.size());
  Serial.println("---------------------");
  Serial.println();
  storage.load();  
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void loop() {
  unsigned long __millis = millis();
  if (__millis >= update_data_time) {
    storage.uptime += __millis - last_update_time;
    storage.save();
    last_update_time = __millis;
    update_data_time = __millis + 15000;  // Save new time every 15 seconds
    print_values();
  }
}

