#include <Arduino.h>
#include <erom.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Define a 'Storage' class that will manage EEPROM related data
class Storage : public erom::Storage {
protected:
  // Define rules for loading storage into RAM
  virtual void OnLoad() { volume.load(); brightness.load(); }
  // Define rules for saving storage into EEPROM
  virtual void OnSave() { volume.save(); brightness.save(); }

public:
  // Storage data/variables
  erom::Entry<float> volume;
  erom::Entry<short> brightness;

  // Binding/allocating variable to EEPROM memory addresses
  Storage() { issue(volume); issue(brightness); }
};

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Create 'storage' object for managing data
Storage storage;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void setup() {
  // Initialize hardware
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  delay(3000);

  // Load EEPROM storage data into RAM
  storage.load();
  
  // Print loaded data
  Serial.print("Original volume: ");
  Serial.println(storage.volume);
  Serial.print("Original brightness: ");
  Serial.println(storage.brightness);
  Serial.println("-----------------------");
  
  // Give storage data new values
  storage.volume = analogRead(A0) / 1024.f;
  storage.brightness = map(analogRead(A1), 0, 1024, 0, 255);

  // Print newly set data
  Serial.print("New volume: ");
  Serial.println(storage.volume);
  Serial.print("New brightness: ");
  Serial.println(storage.brightness);
  Serial.println("-----------------------");

  // Store new data to EEPROM
  storage.save();
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //

void loop() { }