#pragma once

#define ERR_NO_ERROR_D						((uint16_t)0x00u)
#define ERR_FILESYSTEM_INIT_FAILED_D		((uint16_t)0x01u)
#define ERR_FILE_OPEN_ERROR_D				((uint16_t)0x02u)
#define ERR_CONFIG_FILE_EMPTY_D				((uint16_t)0x03u)

#define FORMAT_SPIFFS_IF_FAILED_D			((bool)true)



#define GPIO_BLUE_LED_PIN_D					((uint8_t)2u)
#define CUT_OFF_MOSFET_PIN_D				((uint8_t)13u)
#define CHARGE_MOSFET_1_PIN_D				((uint8_t)12u)
#define CHARGE_MOSFET_2_PIN_D				((uint8_t)14u)
#define CAPACITOR_1_VOLTAGE_PIN_D			((uint8_t)25u)
#define SOLAR_OPEN_VOLTAGE_PIN_D			((uint8_t)33u)

#define ADC_MAX_VALUE_D						((uint16_t)4095u)
#define ADC_MIN_VALUE_D						((uint16_t)1u)
#define DIVIDER_CONSTANT_D					((uint16_t)2u)

#define BREAK_IF_RESULT_NOT_OK(x)			if(x != ERR_NO_ERROR_D) break;




#define CONFIG_FILE_PATH_D					("/cfg.json")
#define MAX_JSON_FILESIZE_D					(1024u)
													
