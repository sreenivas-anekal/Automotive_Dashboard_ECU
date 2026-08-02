#include "adc_sensor.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // Initialize ADC Sensor Module
    ADC_Sensor_Init();

    Serial.println("=== Automotive Dashboard: Module 3 (ADC Sensor) Test ===");

    // Test a mock raw ADC reading (e.g., mid-scale)
    uint16_t raw_adc = ADC_Read_Raw(0);
    float voltage = ADC_ConvertToVoltage(raw_adc);
    float fuel_level = ADC_GetFuelLevelPercentage(raw_adc);

    char log_buf[96];
    sprintf(log_buf, "Raw ADC: %u | Voltage: %.2fV | Fuel Level: %.1f%%", raw_adc, voltage, fuel_level);
    Serial.println(log_buf);

    Serial.println("=== Module 3 Test Complete Successfully! ===");
}

void loop() {
    // Continuously monitor simulated sensor updates every second
    uint16_t raw_adc = ADC_Read_Raw(0);
    float voltage = ADC_ConvertToVoltage(raw_adc);
    float fuel_level = ADC_GetFuelLevelPercentage(raw_adc);

    char log_buf[96];
    sprintf(log_buf, "Live Sensor -> Raw: %u | Voltage: %.2fV | Fuel: %.1f%%", raw_adc, voltage, fuel_level);
    Serial.println(log_buf);

    delay(1000);
}
