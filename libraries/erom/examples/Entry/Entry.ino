#include <Arduino.h>
#include <erom.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

erom::Entry<int>   ei(0);
erom::Entry<long>  el(ei.size);
erom::Entry<float> ef(ei.size + el.size);
const size_t e_sz = ei.size + el.size + ef.size;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void set_values() { ei = 12345; el = 1234567890; ef = 3.1415f; }
void restore_values() { ei.load(); el.load(); ef.load(); }

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void print_value(const char *aTitle) {
  Serial.print(aTitle);
  Serial.print(" (");
  Serial.print(e_sz);
  Serial.print(" bytes): ");
  Serial.print(ei.value);
  Serial.print(", ");
  Serial.print(el.value);
  Serial.print(", ");
  Serial.print(ef.value);
  Serial.println();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void read_eeprom(bool aRestore = true) {
  if (aRestore) restore_values();
  print_value("EEPROM data is");
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void write_eeprom() {
  set_values();
  print_value("Storing data to EEPROM");
  ei.save(true); el.save(true); ef.save(true);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void update_eeprom() {
  set_values();
  print_value("Updating EEPROM data ");
  ei.save(); el.save(); ef.save();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void clear_eeprom() {
  Serial.print("Clearing EEPROM data (");
  Serial.print(e_sz);
  Serial.print(" bytes)...");
  Serial.println();

  for (size_t __i = 0; __i < e_sz; __i++) erom::access.write_byte(__i, 0);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void print_help() {
  Serial.println("Usage:");
  Serial.println(" C - clear EEPROM memory");
  Serial.println(" R - read EEPROM memory");
  Serial.println(" U - update EEPROM memory (write only what is needed)");
  Serial.println(" W - write EEPROM memory");
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
      case 'R': read_eeprom();   break;
      case 'U': update_eeprom(); break;
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
  delay(1000);

  Serial.print("EEPROM Size: "); Serial.println(erom::Access::device_memory_size());
  while (Serial.available() > 0) Serial.read();
  randomSeed(analogRead(A0) * analogRead(A1) * analogRead(A2) * analogRead(A3));

  print_help();
  read_eeprom(false);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void loop() { /* Do Nothing */ }

