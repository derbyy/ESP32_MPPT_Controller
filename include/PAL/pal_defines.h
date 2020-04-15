#pragma once
#include "Arduino.h"

#define ERR_NO_ERROR_D						((uint16_t)0x0000u)
#define ERR_FILESYSTEM_INIT_FAILED_D		((uint16_t)0x0001u)
#define ERR_FILE_OPEN_ERROR_D				((uint16_t)0x0002u)
#define ERR_CONFIG_FILE_EMPTY_D				((uint16_t)0x0003u)
#define ERR_FILE_TOO_BIG_D					((uint16_t)0x0004u)
#define ERR_JSON_DESERIALIZATION_D			((uint16_t)0x0005u)
#define ERR_CONNECTIO_FAILED_D				((uint16_t)0x0006u)

#define FORMAT_SPIFFS_IF_FAILED_D			((bool)true)



#define GPIO_BLUE_LED_PIN_D					((uint8_t)2u)
#define CUT_OFF_MOSFET_PIN_D				((uint8_t)27u)
#define CHARGE_MOSFET_1_PIN_D				((uint8_t)25u)
#define CHARGE_MOSFET_2_PIN_D				((uint8_t)26u)          //26
/* TODO Change name of define */
#define CAPACITOR_1_VOLTAGE_PIN_D			((uint8_t)15u)
/* TODO Change name of define */
#define CAPACITOR_2_VOLTAGE_PIN_D			((uint8_t)34u)
/* TODO Change name of define */
#define SOLAR_OPEN_VOLTAGE_PIN_D			((uint8_t)35u)

#define BREAK_IF_RESULT_NOT_OK(x)			if(x != ERR_NO_ERROR_D) break;

/* Structure to store credentials data */
typedef struct
{
	String strSsid;
	String strPassword;
}Config_T;
													
