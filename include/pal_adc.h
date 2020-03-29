#include "Arduino.h"
#include "pal_defines.h"

#define ADC_MAX_VALUE_D						((uint16_t)4095u)
#define ADC_MAX_NUM_OF_SAMPLES_D            ((uint16_t)10u)
#define ADC_MIN_VALUE_D						((uint16_t)1u)
#define DIVIDER_CONSTANT_D					((uint16_t)2u)




/* Function used to voltage measurement via ADC inputs */
extern uint16_t pal_adc_u16ReadVoltage(uint8_t u8AdcInput);
