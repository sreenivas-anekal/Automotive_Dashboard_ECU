#ifndef ADC_SENSOR_H
#define ADC_SENSOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_SENSOR_MAX_VALUE      4095U
#define ADC_SENSOR_REFERENCE_VOLT 3.3f

void ADC_Sensor_Init(void);

uint16_t ADC_Read_Raw(uint8_t channel);

float ADC_ConvertToVoltage(uint16_t raw_value);

float ADC_GetFuelLevelPercentage(uint16_t raw_value);

#ifdef __cplusplus
}
#endif

#endif /* ADC_SENSOR_H */
