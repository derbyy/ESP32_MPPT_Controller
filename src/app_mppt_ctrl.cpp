#include "app_mppt_ctrl.h"

/********************************* Static module variables *********************************/
/* Module static structure holds voltage values for all mppt components */
MPPT_CTRL_VOLTAGE_T app_mppt_ctrl_sComponentVoltages = { 0u, 0u, 0u };
/* Module static variable holds switch times for normal direction */
MPPT_CTRL_SWITCH_TIMES_T app_mppt_ctrl_sSwitchTimesNormal = { 0u, 0u };
/* Module static variable holds switch times for reverse direction */
MPPT_CTRL_SWITCH_TIMES_T app_mppt_ctrl_sSwitchTimesReverse = { 0u, 0u };
/*******************************************************************************************/

/******************************** Static module functions **********************************/
/* Module function used to read voltage from MPPT components */
static uint16_t app_mppt_ctrl_u16ReadComponentVoltage_mV(void);
/* Module function used to calculate maximal power point of solar cell */
static uint16_t app_mppt_ctrl_u16CalculateMaxPowerPoint(uint16_t u16InputOpenVoltage);
/* Module function used to calculate switching times for switch mosfets */
static uint16_t app_mppt_ctrl_u16CalculateTimes(MPPT_CTRL_SWITCH_TYPE_T eSwitchType);
/* Module function used to pump energy from 5F capacitor bank into 12.5F capacitor bank */
static uint16_t app_mppt_ctrl_u16PumpNormal(void);
/* Module function used to pump energy from 12.5F capacitor bank into 5F capacitor bank */
static uint16_t app_mppt_ctrl_u16PumpReverse(void);
/*******************************************************************************************/


void app_mppt_ctrl_vInitialize(void)
{
    /* TODO change to esp code */
    /* Initialize GPIO pins to output mode */
    pinMode(CUT_OFF_MOSFET_PIN_D, OUTPUT);
    pinMode(CHARGE_MOSFET_1_PIN_D, OUTPUT);
    pinMode(CHARGE_MOSFET_2_PIN_D, OUTPUT);

    /* Configure voltage cut-off mosfet pin as output - High */
    digitalWrite(CUT_OFF_MOSFET_PIN_D, HIGH);

    /* Configure voltage charge mosfet 1 pin as output - High */
    digitalWrite(CHARGE_MOSFET_1_PIN_D, HIGH);

    /* Configure voltage charge mosfet 2 pin as output - Low */
    digitalWrite(CHARGE_MOSFET_2_PIN_D, LOW);

    /* Configure ADC pins */
    adcAttachPin(CAPACITOR_1_VOLTAGE_PIN_D);
    adcAttachPin(CAPACITOR_2_VOLTAGE_PIN_D);
	adcAttachPin(SOLAR_OPEN_VOLTAGE_PIN_D);

    /* TODO Move to ADC configuration - pal_adc.c */
    /* Set ADC clock divider */
    analogSetClockDiv(1);

    /* TODO Move to pal_ADC Make calibration */
    Serial.println("--------------- Calibration ---------------");
    for(uint8_t u8Count = 0u; u8Count < 3; u8Count++)
    {
        (void)app_mppt_ctrl_u16ReadComponentVoltage_mV();
    }
    Serial.println("-------------------------------------------");
}

void app_mppt_ctrl_vTask(void)
{
    uint16_t u16Result = 0u;

    /* Read 10F & 25F & solar cell open voltage */
    u16Result = app_mppt_ctrl_u16ReadComponentVoltage_mV();
    if(u16Result != ERR_NO_ERROR_D)
    {
        //FIXME make return function type
        //return u16Result;
    }

    if(app_mppt_ctrl_sComponentVoltages.u16VoltageCap_5F_mV > app_mppt_ctrl_sComponentVoltages.u16MaxPowerPointVal)
    {
        /* Pump energy from 10F capacitor bank into 25F capacitor bank until voltage on 10F cap will be the same as mppt */
        while (app_mppt_ctrl_sComponentVoltages.u16VoltageCap_5F_mV >= app_mppt_ctrl_sComponentVoltages.u16MaxPowerPointVal)
        {
            /* Calculate switching times for charging mosfets */
            u16Result = app_mppt_ctrl_u16CalculateTimes(MPPT_CTRL_SWITCH_TYPE_NORMAL_E);
            if(u16Result != ERR_NO_ERROR_D)
            {
                break;
            }
            /* Make 1 pumpung cycle */
            //u16Result = app_mppt_ctrl_u16PumpNormal();
            if(u16Result != ERR_NO_ERROR_D)
            {
                break;
            }
            /* Measure voltages again */
            u16Result = app_mppt_ctrl_u16ReadComponentVoltage_mV();
            if(u16Result != ERR_NO_ERROR_D)
            {
                break;
            }

            delay(25);

        }       
    }
    else
    {
        /* Pump energy from 25F capacitor bank into 10F capacitor bank until voltage on 10F cap will be the same as mppt */
        //do
        //{
            /* TODO 1. Calculate reverse switching times for charging mosfets */
            /* TODO 2. Make 1 reverse pumpung cycle */
            /* TODO 3. Measure voltages again */
        //} while (app_mppt_ctrl_sComponentVoltages.u16VoltageCap_5F_mV < app_mppt_ctrl_sComponentVoltages.u16MaxPowerPointVal);
        
    }
}

static uint16_t app_mppt_ctrl_u16ReadComponentVoltage_mV(void)
{
    uint16_t u16Voltage = 0u;

    /* Read voltage of 10F capacitor bank */
    app_mppt_ctrl_sComponentVoltages.u16VoltageCap_5F_mV = pal_adc_u16ReadVoltage(CAPACITOR_1_VOLTAGE_PIN_D);

    /* Read voltage of 25F capacitor bank */
    //app_mppt_ctrl_sComponentVoltages.u16VoltageCap_25F_mV = 1;//pal_adc_u16ReadVoltage(CAPACITOR_2_VOLTAGE_PIN_D);
    app_mppt_ctrl_sComponentVoltages.u16VoltageCap_12_5F_mV = pal_adc_u16ReadVoltage(CAPACITOR_2_VOLTAGE_PIN_D);


    /* Cut of the load from solar cell */
    digitalWrite(CUT_OFF_MOSFET_PIN_D, LOW);

    /* Wait 25 ms to stabilize the voltage */
    delay(APP_MPPT_CTRL_STABILIZE_TIME_D);

    /* Read open voltage of solar cell */
    u16Voltage = pal_adc_u16ReadVoltage(SOLAR_OPEN_VOLTAGE_PIN_D);

    /* Connect load back to solar cell */
    digitalWrite(CUT_OFF_MOSFET_PIN_D, HIGH);

    /* Calculate maximal power point of solar cell */
    app_mppt_ctrl_sComponentVoltages.u16MaxPowerPointVal = app_mppt_ctrl_u16CalculateMaxPowerPoint(u16Voltage);

#if _DEBUG
    Serial.println("Cap 1: " + String(app_mppt_ctrl_sComponentVoltages.u16VoltageCap_5F_mV) + " mV, Cap 2: " + 
        app_mppt_ctrl_sComponentVoltages.u16VoltageCap_12_5F_mV + " mV, MPPT: " + app_mppt_ctrl_sComponentVoltages.u16MaxPowerPointVal);
#endif

    /* TODO Return error code */
    return 0;
}


static uint16_t app_mppt_ctrl_u16CalculateMaxPowerPoint(uint16_t u16InputOpenVoltage)
{
    /* Calculate maximal power point from solar cell open voltage */
    return (uint16_t)((float_t)u16InputOpenVoltage * APP_MPPT_CTRL_PROPORTIONAL_CONSTANT_D);
}


static uint16_t app_mppt_ctrl_u16CalculateTimes(MPPT_CTRL_SWITCH_TYPE_T eSwitchType)
{
    uint16_t u16Result = 0u;
    float_t fTemp = 0u;

    /* Select switch type */
    switch(eSwitchType)
    {
        case MPPT_CTRL_SWITCH_TYPE_NORMAL_E:
        {
            /* Calculate switch time 1 : t1 <= (induktance * induktor max current load) / 10F capacitor voltage  */
             fTemp = (float_t)(APP_MPPT_CTRL_INDUCTANCE_D * APP_MPPT_CTRL_MAX_CURRENT_LOAD_D) / 
                (float_t)app_mppt_ctrl_sComponentVoltages.u16VoltageCap_5F_mV;

            /* Apply correction value and convert to micro seconds */
            app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_1_us = (uint32_t)(fTemp * (float_t)1000) - APP_MPPT_CTRL_TIME_CORRECTION_D;

            /* Calculate switch time 2 : t2 <= t1 * (10F capacitor voltage / 25F capacitor voltage) */
            fTemp = ((float_t)app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_1_us / (float_t)1000) *  
                (float_t)(app_mppt_ctrl_sComponentVoltages.u16VoltageCap_5F_mV / app_mppt_ctrl_sComponentVoltages.u16VoltageCap_12_5F_mV);

            /* Apply correction value and convert to micro seconds */
            app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_2_us = (uint32_t)(fTemp * (float_t)1000) - APP_MPPT_CTRL_TIME_CORRECTION_D;
#if _DEBUG
            Serial.println("Time 1 :" + String(app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_1_us) + " us, Time 2 : " +
                String(app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_2_us) + " us");
#endif
            break;
        }
        case MPPT_CTRL_SWITCH_TYPE_REVERSE_E:
        {
            /* Calculate switch time 1 : t1 <= (induktance * induktor max current load) / 25F capacitor voltage  */
            fTemp = (float_t)(APP_MPPT_CTRL_INDUCTANCE_D * APP_MPPT_CTRL_MAX_CURRENT_LOAD_D) / 
                (float_t)app_mppt_ctrl_sComponentVoltages.u16VoltageCap_12_5F_mV;

            /* Apply correction value and convert to micro seconds */
            app_mppt_ctrl_sSwitchTimesReverse.u32SwitchTime_1_us = (uint32_t)(fTemp * (float_t)1000) - APP_MPPT_CTRL_TIME_CORRECTION_D;

            /* Calculate switch time 2 : t2 <= t1 * (25F capacitor voltage / 10F capacitor voltage) */
            fTemp = ((float_t)app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_1_us / (float_t)1000) * 
                (float_t)(app_mppt_ctrl_sComponentVoltages.u16VoltageCap_12_5F_mV / app_mppt_ctrl_sComponentVoltages.u16VoltageCap_5F_mV);

            /* Apply correction value and convert to micro seconds */
            app_mppt_ctrl_sSwitchTimesReverse.u32SwitchTime_2_us = (uint32_t)(fTemp * (float_t)1000) - APP_MPPT_CTRL_TIME_CORRECTION_D;
#if _DEBUG
            Serial.println("Time 1 :" + String(app_mppt_ctrl_sSwitchTimesReverse.u32SwitchTime_1_us) + " us, Time 2 : " +
                String(app_mppt_ctrl_sSwitchTimesReverse.u32SwitchTime_2_us) + " us");
#endif
            break;
        }
        default:
        {
            /* TODO Add return code */
            u16Result = 1u;
            break;
        }
    }

    return u16Result;
}


static uint16_t app_mppt_ctrl_u16PumpNormal(void)
{
    uint16_t u16Result = 0u;

    /* Turn ON first charging mosfet */
    digitalWrite(CHARGE_MOSFET_1_PIN_D, LOW);

    /* Wait specific time */
    //delay(app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_1_us);
    delayMicroseconds(app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_1_us);

    /* Turn OFF first charginf mosfet */
    digitalWrite(CHARGE_MOSFET_1_PIN_D, HIGH);

    delayMicroseconds(1);

    /* Turn ON second charging mosfet */
    digitalWrite(CHARGE_MOSFET_2_PIN_D, HIGH);

    /* Wait specific time */
    //delay(app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_2_us);
    delayMicroseconds(app_mppt_ctrl_sSwitchTimesNormal.u32SwitchTime_2_us);

     /* Turn OFF second charginf mosfet */
    digitalWrite(CHARGE_MOSFET_2_PIN_D, LOW);

    return u16Result;
}


static uint16_t app_mppt_ctrl_u16PumpReverse(void)
{
    uint16_t u16Result = 0u;

    /* Turn ON second charging mosfet */
    digitalWrite(CHARGE_MOSFET_2_PIN_D, HIGH);

    /* Wait specific time */
    delay(app_mppt_ctrl_sSwitchTimesReverse.u32SwitchTime_1_us);

     /* Turn OFF second charginf mosfet */
    digitalWrite(CHARGE_MOSFET_2_PIN_D, LOW);

    /* Turn ON first charging mosfet */
    digitalWrite(CHARGE_MOSFET_1_PIN_D, LOW);

    /* Wait specific time */
    delay(app_mppt_ctrl_sSwitchTimesReverse.u32SwitchTime_2_us);

    /* Turn OFF first charginf mosfet */
    digitalWrite(CHARGE_MOSFET_1_PIN_D, HIGH);

    return u16Result;
}


