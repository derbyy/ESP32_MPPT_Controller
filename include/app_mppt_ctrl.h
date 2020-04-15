#include "PAL/pal_adc.h"
/******************************************* Defines ******************************************/
/* Value defines proportional constant for open voltage MPPT tracking method (0.71 - 0.80) */
#define APP_MPPT_CTRL_PROPORTIONAL_CONSTANT_D                         ((float_t)0.76)
/* Value defines inductor maximal currend load in mili Amps */
#define APP_MPPT_CTRL_MAX_CURRENT_LOAD_D                              ((uint16_t)800)
/* Value defines inductor inductance in mili Henry */
#define APP_MPPT_CTRL_INDUCTANCE_D                                    ((uint16_t)1)
/* Value defines value of minimal voltage of 10F capacitor bank to run MPPT tracking in mili Volts */
#define APP_MPPT_CTRL_MIN_CAP_10F_VOLTAGE_D                           ((uint16_t)2300)  
/* Value defines value of minimal voltage of 25F capacitor bank to run MPPT tracking in mili Volts */
#define APP_MPPT_CTRL_MIN_CAP_25F_VOLTAGE_D                           ((uint16_t)700)
/* Value defines time to stabilize voltage after load was cut of from solar cell */
#define APP_MPPT_CTRL_STABILIZE_TIME_D                                (25u)
/* Value defines time correction constant */
#define APP_MPPT_CTRL_TIME_CORRECTION_D                               ((uint32_t)10u)                                

/**********************************************************************************************/

/*********************************** Module type definitions **********************************/
/* Structure to hold voltage values for mppt components */
typedef struct
{
    uint16_t u16VoltageCap_5F_mV;
    uint16_t u16VoltageCap_12_5F_mV;
    uint16_t u16MaxPowerPointVal;
}MPPT_CTRL_VOLTAGE_T;

/* Structure to hold switching times for charging mosfets */
typedef struct 
{
   uint32_t u32SwitchTime_1_us;
   uint32_t u32SwitchTime_2_us;
}MPPT_CTRL_SWITCH_TIMES_T;

/* Enumeration to describe switch time type */
typedef enum
{
    MPPT_CTRL_SWITCH_TYPE_NORMAL_E = 0u,
    MPPT_CTRL_SWITCH_TYPE_REVERSE_E
}MPPT_CTRL_SWITCH_TYPE_T;

/**********************************************************************************************/

/************************************ Function definitions ************************************/

/* Function used to initialize mppt module */
extern void app_mppt_ctrl_vInitialize(void);
/* Function used to track maximal power point of solar cell */
extern void app_mppt_ctrl_vTask(void);

/**********************************************************************************************/