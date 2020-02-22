// pal_WifiConnection.h

#ifndef _PAL_WIFICONNECTION_h
#define _PAL_WIFICONNECTION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "pal_Defines.h"
#include "WebServer.h"
#include "WiFi.h"

/* Function used to Wifi network initialization */
extern uint16_t pal_wifi_u16ConnectToWifi(Config_T pCredentials);

#endif

