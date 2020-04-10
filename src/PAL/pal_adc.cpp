#include "pal_adc.h"

/***************************** STATIC FUNCTIONS *******************************/
/* Function used to recalculate ADC value to voltage in mV */
static uint16_t pal_adc_u16CalculateVoltage_mV(uint16_t u16InputSample);
/* Function used to apply correction to voltage values */
static uint16_t pal_adc_u16ApplyCorrection(uint16_t u16InputValue_mV);
/* Function used to apply divider to voltage values */
static uint16_t pal_adc_u16ApplyDividerCorrection(uint16_t u16InputValue_mV);
/******************************************************************************/


/******************************************************************************
* Parameters		:       uint16_t                                          *
* Return value		:		uint16_t								          *
* Description		:		Function used to recalculate ADC value            *
*							to voltage in mV				                  *
/******************************************************************************/
static uint16_t pal_adc_u16CalculateVoltage_mV(uint16_t u16InputSample)
{
    double_t res = 0.0;

    /* Polynomial curve of EPS32 ADC measurements */
    const double_t coeffs[] = { 91.8, 0.9, -1e-4, 4e-08, -6e-12 };

    if(u16InputSample <= 0u)
    {
        return 0u;
    }

    /* Use Horner algorithm to calculate voltage in mV from polynomial curve */
    for (int8_t s8Counter = (sizeof(coeffs) / sizeof(double_t) - 1); s8Counter >= 0; s8Counter--)
    {
        res = res * (double_t)u16InputSample + coeffs[s8Counter];
    }
#if _DEBUG
		//Serial.println("Raw voltage: " + String(res));
#endif

    return (uint16_t)res;
}

/******************************************************************************
* Parameters		:       uint16_t                                          *
* Return value		:		uint16_t								          *
* Description		:		Function used to apply correction to 	          *
*							voltage values				                      *
/******************************************************************************/
static uint16_t pal_adc_u16ApplyCorrection(uint16_t u16InputValue_mV)
{
    const uint16_t au16CorrectionTable[] = {23u, 2u, 20u, 22u};
    uint16_t u16CorrectedValue;

    /* Correct values */
    if((u16InputValue_mV >= 120u) && (u16InputValue_mV < 400u))
    {
        u16CorrectedValue = u16InputValue_mV + au16CorrectionTable[0];
    }
    else if((u16InputValue_mV >= 2000u) && (u16InputValue_mV < 2900u))
    {
        u16CorrectedValue = u16InputValue_mV + au16CorrectionTable[2];
    }
    else if((u16InputValue_mV >= 2900u) && (u16InputValue_mV < 3150u))
    {
        u16CorrectedValue = u16InputValue_mV - au16CorrectionTable[3];
    }
    else
    {
        u16CorrectedValue = u16InputValue_mV;
    }

#if _DEBUG
		//Serial.println("After correction: " + String(u16CorrectedValue));
#endif

    return u16CorrectedValue;
}

/******************************************************************************
* Parameters		:       uint16_t                                          *
* Return value		:		uint16_t								          *
* Description		:		Function used to apply divider to		          *
*							voltage values				                      *
/******************************************************************************/
static uint16_t pal_adc_u16ApplyDividerCorrection(uint16_t u16InputValue_mV)
{
    return u16InputValue_mV * DIVIDER_CONSTANT_D;
}


/******************************************************************************
* Parameters		:       uint16_t                                          *
* Return value		:		uint16_t								          *
* Description		:		Function used to apply divider to		          *
*							voltage values				                      *
/******************************************************************************/
uint16_t pal_adc_u16ReadVoltage(uint8_t u8AdcInput)
{
    uint16_t u16AdcSamples = 0u;
    uint16_t u16Voltage_mV = 0u;

    /* Read specific ADC input more times to get accurate result */
    for(uint8_t u8Counter = 0u; u8Counter < ADC_MAX_NUM_OF_SAMPLES_D; u8Counter++)
    {
        u16AdcSamples += analogRead(u8AdcInput);
    }

    /* Get average result */
    uint16_t u16AdcSampleAverage = u16AdcSamples / ADC_MAX_NUM_OF_SAMPLES_D;
#if _DEBUG
    //Serial.println("ADC value : " + String(u16AdcSampleAverage));
#endif

    /* Recalculate average ADC sample to voltage in milivolts */
    u16Voltage_mV = pal_adc_u16CalculateVoltage_mV(u16AdcSampleAverage);

    /* Apply non linear corrections to voltage calculation */
    u16Voltage_mV = pal_adc_u16ApplyCorrection(u16Voltage_mV);

    /* Apply voltage divider */
    u16Voltage_mV = pal_adc_u16ApplyDividerCorrection(u16Voltage_mV);

#if _DEBUG
		Serial.println("final voltage: " + String(u16Voltage_mV));
#endif

    return u16Voltage_mV;
}