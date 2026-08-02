#include "eeprom_storage.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    EEPROM_Storage_Init();
    Serial.println("=== Module 5: EEPROM Storage Test ===");

    // Read initial odometer
    uint32_t current_odometer = EEPROM_GetOdometer();
    char buf[64];
    sprintf(buf, "Loaded Odometer from Non-Volatile Memory: %u km", (unsigned int)current_odometer);
    Serial.println(buf);

    // Simulate driving and updating mileage
    uint32_t new_mileage = current_odometer + 15;
    EEPROM_SaveOdometer(new_mileage);
    sprintf(buf, "Updated Odometer Saved: %u km", (unsigned int)EEPROM_GetOdometer());
    Serial.println(buf);

    Serial.println("=== Module 5 Test Complete Successfully! ===");
}

void loop() {}
