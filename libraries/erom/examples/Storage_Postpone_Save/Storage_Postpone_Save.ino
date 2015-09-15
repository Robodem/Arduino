#include <Arduino.h>
#include <erom.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void show_data(const char *aTitle);

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

class Storage : public erom::Storage {
protected:
  virtual void OnLoad() { A0.load(); A1.load(); }
  virtual void OnSave() { show_data("Storing data to EEPROM"); A0.save(); A1.save(); }
  virtual void OnClear() { A0 = 0; A1 = 0; }

public:
  erom::Entry<int> A0, A1;

  Storage() : erom::Storage() { issue(A0); issue(A1); }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

Storage storage;
bool recording = false;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void show_data(const char *aTitle) {
  Serial.print(aTitle);
  Serial.print(" (");
  Serial.print(storage.size());
  Serial.print(" bytes): A0=");
  Serial.print(storage.A0.value);
  Serial.print("; A1=");
  Serial.print(storage.A1.value);
  Serial.println();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void read_eeprom(bool aRestore = true) {
  if (aRestore) { storage.load(); show_data("EEPROM data is"); }
  else show_data("RAM data is");
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void clear_eeprom() {
  Serial.println("Clearing data...");
  storage.clear();
  read_eeprom();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void print_help() {
  Serial.println("Usage:");
  Serial.println(" C - clear EEPROM memory");
  Serial.println(" R - read EEPROM memory");
  Serial.println(" V - values currently in RAM");
  Serial.println(" S - start recording data");
  Serial.println(" X - stop recording data");
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
      case 'C': clear_eeprom();     break;
      case 'R': read_eeprom();      break;
      case 'V': read_eeprom(false); break;
      case 'S': recording = true;   Serial.println("Recording started..."); break;
      case 'X': recording = false;  Serial.println("Recording stopped.");   break;
      default:  show_help();        break;
    }

    while (Serial.available() > 0) Serial.read();
    delay(500);
  }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  delay(1000);

  Serial.print("EEPROM Size: "); Serial.println(erom::Access::device_memory_size());
  while (Serial.available() > 0) Serial.read();
  randomSeed(analogRead(A0) * analogRead(A1) * analogRead(A2) * analogRead(A3));

  print_help();
  read_eeprom();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void loop() {
  if (recording) {
    storage.A0 = analogRead(A0);
    storage.A1 = analogRead(A1);
    storage.postpone_save(3000);  // Save data to EEPROM every 3 seconds
  }

  storage.tick(); // Calls 'save()' when needed (i.e., every 3 seconds)
}

