#include "adc_sensor.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    ADC_Sensor_Init();
    Serial.println("=== Module 3: ADC Sensor Driver Test ===");

    uint16_t raw_adc = ADC_Read_Raw(0);
    float voltage = ADC_ConvertToVoltage(raw_adc);
    float fuel_level = ADC_GetFuelLevelPercentage(raw_adc);

    char buf[96];
    sprintf(buf, "Raw ADC: %u | Voltage: %.2fV | Fuel: %.1f%%", raw_adc, voltage, fuel_level);
    Serial.println(buf);
}

void loop() {
    uint16_t raw_adc = ADC_Read_Raw(0);
    float voltage = ADC_ConvertToVoltage(raw_adc);
    float fuel_level = ADC_GetFuelLevelPercentage(raw_adc);

    char buf[96];
    sprintf(buf, "Live -> Raw: %u | Voltage: %.2fV | Fuel: %.1f%%", raw_adc, voltage, fuel_level);
    Serial.println(buf);
    delay(1000);
}
