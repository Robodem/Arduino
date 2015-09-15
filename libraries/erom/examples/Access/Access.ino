#include <Arduino.h>
#include <erom.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

const size_t eeprom_data_sz      = 21;
byte eeprom_data[eeprom_data_sz] = { "Robodem EEPROM test." };

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void read_eeprom() {
  Serial.print("EEPROM data is (");
  Serial.print(eeprom_data_sz);
  Serial.print(" bytes): ");

  for (size_t __i = 0; __i < eeprom_data_sz; __i++) {
    char __c = (char)erom::access.read_byte(__i);
    Serial.print(__c < 32 ? '.' : __c);
  }

  Serial.println();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void write_eeprom() {
  Serial.print("Storing data to EEPROM (");
  Serial.print(eeprom_data_sz);
  Serial.print(" bytes): ");

  for (size_t __i = 0; __i < eeprom_data_sz; __i++) {
    char __c = (char)eeprom_data[__i];
    Serial.print(__c < 32 ? '.' : __c);
    erom::access.write_char(__i, __c);
  }

  Serial.println();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void update_eeprom() {
  Serial.print("Updating data in EEPROM (");
  Serial.print(eeprom_data_sz);
  Serial.print(" bytes): ");

  for (size_t __i = 0; __i < eeprom_data_sz; __i++) {
    char __c = (char)eeprom_data[__i];
    Serial.print(__c < 32 ? '.' : __c);
    erom::access.update_char(__i, __c);
  }

  Serial.println();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void trash_eeprom() {
  byte __data[eeprom_data_sz];
  for (size_t __i = 0; __i < eeprom_data_sz; __i++)
    __data[__i] = random(5) ? eeprom_data[__i] : random('!', '~');

  Serial.print("EEPROM trash data (");
  Serial.print(eeprom_data_sz);
  Serial.print(" bytes): ");

  for (size_t __i = 0; __i < eeprom_data_sz; __i++) {
    char __c = (char)__data[__i];
    Serial.print(__c < 32 ? '.' : __c);
    erom::access.write_char(__i, __c);
  }

  Serial.println();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void clear_eeprom() {
  Serial.print("Clearing EEPROM data (");
  Serial.print(eeprom_data_sz);
  Serial.println(" bytes)...");

  for (size_t __i = 0; __i < eeprom_data_sz; __i++) {
    erom::access.write_byte(__i, 0);
  }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void print_help() {
  Serial.println("Usage:");
  Serial.println(" C - clear EEPROM memory");
  Serial.println(" R - read EEPROM memory");
  Serial.println(" T - trash EEPROM memory (random chars at random places)");
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
      case 'T': trash_eeprom();  break;
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
  read_eeprom();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void loop() { /* Do Nothing */ }

