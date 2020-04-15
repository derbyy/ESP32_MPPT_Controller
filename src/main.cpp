/*
 Name:		ESP32_Mppt.ino
 Created:	8/11/2019 9:24:44 AM
 Author:	derby
*/
#include "Arduino.h"
#include "PAL/pal_defines.h"
#include "PAL/pal_adc.h"
#include "app_mppt_ctrl.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"


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

    app_mppt_ctrl_vInitialize();    
}

void loop() 
{
    /* Do MPPT task every 5 seconds */
    app_mppt_ctrl_vTask();
    /* TODO Use sleep mode in the future */
    delay(5000);
}