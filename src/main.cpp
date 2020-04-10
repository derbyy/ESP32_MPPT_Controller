/*
 Name:		ESP32_Mppt.ino
 Created:	8/11/2019 9:24:44 AM
 Author:	derby
*/
#include "Arduino.h"
#include "pal_defines.h"
#include "pal_adc.h"


/* Define variable to hold error state */
static uint16_t u16Result = ERR_NO_ERROR_D;
/* Initialize structure to store network credentials */
static Config_T struct_Credentials;

void setup() 
{
#if _DEBUG
	/* Initialize serial port communication */
	Serial.begin(115200);
#endif

// 	do
// 	{
// 		/* Initialize file system */
// 		u16Result = pal_fs_u16FileSystemInitialization();
// 		BREAK_IF_RESULT_NOT_OK(u16Result)

// 		/* Get network credentials */
// 		u16Result = pal_fs_u16GetCredentials(CONFIG_FILE_PATH_D, &struct_Credentials);
// 		BREAK_IF_RESULT_NOT_OK(u16Result)

// 		/* Connect to local wifi network */
// 		u16Result = pal_wifi_u16ConnectToWifi(struct_Credentials);
// 		BREAK_IF_RESULT_NOT_OK(u16Result)

// 	} while (0);

// 	if (u16Result != ERR_NO_ERROR_D)
// 	{
//         /* Start configuration webpage if configuration file does not exist or is not possible to read file */
// 		if ((u16Result == ERR_CONFIG_FILE_EMPTY_D) || (u16Result == ERR_FILE_OPEN_ERROR_D))
// 		{
// 			/* TODO: Run config site with default ACCESS point */

//             /* TODO: make software reset when device after configuration is done */
//             ESP.restart();
// 		}
// 		else
// 		{
//             /* Make software reset of device */
// #if _DEBUG
// 			Serial.println("Do software reset !!!!");
// 			delay(1000);
// #endif
// 			ESP.restart();
// 		}
// 	}

 	pinMode(GPIO_BLUE_LED_PIN_D, OUTPUT);
    digitalWrite(GPIO_BLUE_LED_PIN_D, LOW);

	/* Configure voltage cut-off mosfet pin as output - High */
	pinMode(CUT_OFF_MOSFET_PIN_D, OUTPUT);
	digitalWrite(CUT_OFF_MOSFET_PIN_D, HIGH);

	/* Configure voltage charge mosfet 1 pin as output - High */
	pinMode(CHARGE_MOSFET_1_PIN_D, OUTPUT);
	digitalWrite(CHARGE_MOSFET_1_PIN_D, HIGH);

	/* Configure voltage charge mosfet 2 pin as output - Low */
	pinMode(CHARGE_MOSFET_2_PIN_D, OUTPUT);
	digitalWrite(CHARGE_MOSFET_2_PIN_D, LOW);


	adcAttachPin(CAPACITOR_1_VOLTAGE_PIN_D);
    adcAttachPin(CAPACITOR_2_VOLTAGE_PIN_D);
	adcAttachPin(SOLAR_OPEN_VOLTAGE_PIN_D);
	analogSetClockDiv(1);

}

void loop() 
{
    uint16_t u16CapInVoltage = 0u;
    uint16_t u16SolarVoltage = 0u;
	/* Cut off load */
	digitalWrite(CUT_OFF_MOSFET_PIN_D, LOW);

	/* Wait for ~25 ms to stabilize voltage */
	delay(25);

    Serial.print("Solar ");

	/* Read open voltage of connected solar panel */
	u16SolarVoltage = pal_adc_u16ReadVoltage(SOLAR_OPEN_VOLTAGE_PIN_D);

	/* Connect load after open voltage measurement is done */
	digitalWrite(CUT_OFF_MOSFET_PIN_D, HIGH);

	Serial.print("Capacitor 1 ");
	
    u16CapInVoltage = pal_adc_u16ReadVoltage(CAPACITOR_1_VOLTAGE_PIN_D);
	delay(2000);
}
