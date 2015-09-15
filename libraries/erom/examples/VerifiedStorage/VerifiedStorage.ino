#include <Arduino.h>
#include <erom.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

class Storage : public erom::VerifiedStorage {
protected:
  // 'VerifiedStorage::On???()' must be called for verification to work
  virtual void OnClear() { VerifiedStorage::OnClear(); i = 12345; l = 1234567890; f = 3.14f; }
  virtual void OnLoad()  { VerifiedStorage::OnLoad();  i.load();  l.load();       f.load();  }
  virtual void OnSave()  { VerifiedStorage::OnSave();  i.save();  l.save();       f.save();  }

public:
  erom::Entry<int>   i;
  erom::Entry<long>  l;
  erom::Entry<float> f;

  Storage() : VerifiedStorage(0xFFF0, 0x0001) { issue(i); issue(l); issue(f); }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

Storage storage;
const size_t e_sz = storage.size();

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void print_values(const char *aTitle) {
  Serial.print(aTitle);
  Serial.print(" (");
  Serial.print(e_sz);
  Serial.print(" bytes): ");
  Serial.print(storage.i.value);
  Serial.print(", ");
  Serial.print(storage.l.value);
  Serial.print(", ");
  Serial.print(storage.f.value);
  Serial.println();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

bool verify_eeprom() {
  bool __result = storage.verify();
  Serial.print("Storage verification: ");
  Serial.println(__result ? "OK" : "FAILED");
  return __result;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void read_eeprom(bool aRestore = true) {
  if (aRestore) storage.load();
  verify_eeprom();
  print_values("EEPROM data is");
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void clear_eeprom() {
  storage.clear();
  print_values("Storing data to EEPROM");
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void destroy_eeprom() {
  Serial.print("Clearing EEPROM data (");
  Serial.print(e_sz);
  Serial.println(" bytes)...");

  for (size_t __i = 0; __i < e_sz; __i++) erom::access.write_byte(__i, 0);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void write_eeprom() {
  // Since we do not fix storage on start and/or can destroy it at any moment
  // manually, we should check if memory is initialized.
  if (!storage.verify()) {
    Serial.println("Storage verification: FAILED");
    Serial.println("Initializing memory...");
    storage.clear(false); // Just initialize in memory, since we are going to save it in a moment manually
  }
  
  storage.i = random(1000, 20000);
  (storage.l = random(1, 65000)) *= 1000;
  (storage.f = random(100, 10000)) /= 100;

  print_values("Saving data to EEPROM");
  storage.save();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void print_help() {
  Serial.println("Usage:");
  Serial.println(" C - clear/initialize EEPROM memory");
  Serial.println(" D - destroy/erase EEPROM memory");
  Serial.println(" R - read EEPROM memory");
  Serial.println(" V - verify EEPROM memory");
  Serial.println(" W - randomize EEPROM memory data (user fields only)");
  Serial.println("\n\n");
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void show_help() {
  static bool _do_show = true;
  if (_do_show) print_help(), _do_show = false;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void serialEvent() {
  if (Serial.available() > 0) {
    char __c = toupper(Serial.read());
    switch (__c) {
      case 'C': clear_eeprom();  break;
      case 'D': destroy_eeprom();  break;
      case 'R': read_eeprom();   break;
      case 'V': verify_eeprom(); break;
      case 'W': write_eeprom();  break;
      default: show_help();
    }

    while (Serial.available() > 0) Serial.read(); 
    delay(500);
  }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void setup() {
  Serial.begin(115200);

  Serial.print("EEPROM Size: "); Serial.println(erom::Access::device_memory_size());
  delay(1000);
  while (Serial.available() > 0) Serial.read(); 
  randomSeed(analogRead(A0) * analogRead(A1) * analogRead(A2) * analogRead(A3));

  print_help();
  Serial.print("Storage size: ");
  Serial.println(storage.size());
  read_eeprom(true);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void loop() { /* Do Nothing */ }

