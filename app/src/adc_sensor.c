#include "adc_sensor.h"

#ifdef ESP_PLATFORM
#include "esp_adc/adc_oneshot.h" // For ESP32 native ADC handling if needed
#endif

void ADC_Sensor_Init(void) {
    // Configures ADC channels for simulation input
}

uint16_t ADC_Read_Raw(uint8_t channel) {
    // For simulation purposes, we can return a mock or middle-scale raw ADC value (0 - 4095 for 12-bit ADC)
    (void)channel;
    return 2048; // Default mid-scale reading (~1.65V on a 3.3V ESP32 ADC)
}

float ADC_ConvertToVoltage(uint16_t raw_value) {
    // ESP32 ADC is typically 12-bit (4095 max) with a 3.3V reference
    return ((float)raw_value / 4095.0f) * 3.3f;
}

float ADC_GetFuelLevelPercentage(uint16_t raw_value) {
    // Map raw ADC (0 to 4095) to fuel tank percentage (0.0% to 100.0%)
    float percentage = ((float)raw_value / 4095.0f) * 100.0f;
    if (percentage > 100.0f) percentage = 100.0f;
    if (percentage < 0.0f) percentage = 0.0f;
    return percentage;
}
