#include "pal_WifiConnection.h"

/********************************************************************
* Parameters		:       Config_T pCredentials                   *
* Return value		:		uint16_t								*
* Description		:		Function used to Wifi network			*
*							initialization							*
/********************************************************************/
uint16_t pal_wifi_u16ConnectToWifi(Config_T pCredentials)
{
	uint16_t u16Result;

#if _DEBUG
	Serial.println("Connecting to local wifi network");
#endif

	/* Connect to local wifi network */
	WiFi.begin(pCredentials.strSsid.c_str(), pCredentials.strPassword.c_str());

	/* Wait until connection is done */
	while (WiFi.status() != WL_CONNECTED)
	{
#if _DEBUG
		Serial.print(".");
#endif
		delay(100);
	}

#if _DEBUG
	Serial.println();
#endif

	if (WiFi.status() == WL_CONNECTED)
	{
#if _DEBUG
		Serial.println("Connected to : " + pCredentials.strSsid);
#endif
		u16Result = ERR_NO_ERROR_D;
	}
	else
	{
#if _DEBUG
		Serial.println("Connection failed");
#endif
		u16Result = ERR_CONNECTIO_FAILED_D;
	}

	return u16Result;
}


