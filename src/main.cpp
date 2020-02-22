/*
 Name:		ESP32_Mppt.ino
 Created:	8/11/2019 9:24:44 AM
 Author:	derby
*/
#include "Arduino.h"
#include "pal_WifiConnection.h"
#include "pal_Time.h"
#include "pal_FileSystem.h"


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

	do
	{
		/* Initialize file system */
		u16Result = pal_fs_u16FileSystemInitialization();
		BREAK_IF_RESULT_NOT_OK(u16Result)

		/* Get network credentials */
		u16Result = pal_fs_u16GetCredentials(CONFIG_FILE_PATH_D, &struct_Credentials);
		BREAK_IF_RESULT_NOT_OK(u16Result)

		/* Connect to local wifi network */
		u16Result = pal_wifi_u16ConnectToWifi(struct_Credentials);
		BREAK_IF_RESULT_NOT_OK(u16Result)

	} while (0);

	if (u16Result != ERR_NO_ERROR_D)
	{
		if (u16Result == ERR_CONFIG_FILE_EMPTY_D)
		{
			/* TODO: Run config site with default ACCESS point */
		}
		else
		{

#if _DEBUG
			Serial.println("Do software reset !!!!");
			delay(1000);
#endif
			ESP.restart();
		}
	}

	pinMode(GPIO_BLUE_LED_PIN_D, OUTPUT);
	digitalWrite(GPIO_BLUE_LED_PIN_D, LOW);

	/* Configure voltage cut-off mosfet pin as output - High */
	//pinMode(CUT_OFF_MOSFET_PIN, OUTPUT);
	//digitalWrite(CUT_OFF_MOSFET_PIN, HIGH);

	/* Configure voltage charge mosfet 1 pin as output - High */
	//pinMode(CHARGE_MOSFET_1_PIN, OUTPUT);
	//digitalWrite(CHARGE_MOSFET_1_PIN, HIGH);

	///* Configure voltage charge mosfet 2 pin as output - Low */
	//pinMode(CHARGE_MOSFET_2_PIN, OUTPUT);
	//digitalWrite(CHARGE_MOSFET_2_PIN, LOW);

	//adcAttachPin(CAPACITOR_1_VOLTAGE_PIN);
	//adcAttachPin(SOLAR_OPEN_VOLTAGE_PIN);
	//analogSetClockDiv(255);
}

void loop() 
{
	///* Cut off load */
	//digitalWrite(CUT_OFF_MOSFET_PIN, LOW);

	///* Wait for ~10 ms to stabilize voltage */
	//delay(50);

	//Serial.print("Solar : ");

	///* Read open voltage of connected solar panel */
	//(void)ReadVoltage(CUT_OFF_MOSFET_PIN);

	///* Connect load after open voltage measurement is done */
	////digitalWrite(CUT_OFF_MOSFET_PIN, HIGH);

	//Serial.print("Cap : ");
	//(void)ReadVoltage(CAPACITOR_1_VOLTAGE_PIN);
	//delay(2000);
}


/* ADC readings v voltage
 *  y = -0.000000000009824x3 + 0.000000016557283x2 + 0.000854596860691x + 0.065440348345433
 // Polynomial curve match, based on raw data thus:
 *   464     0.5
 *  1088     1.0
 *  1707     1.5
 *  2331     2.0
 *  2951     2.5
 *  3775     3.0
 *
 */
float_t ReadVoltage(byte pin) 
{
	uint16_t u16CapacitorADCValue;
	double_t f32Voltage = 0.0;

	/* Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095 */
	u16CapacitorADCValue = analogRead(pin); 

	if ((u16CapacitorADCValue < ADC_MIN_VALUE_D) || (u16CapacitorADCValue > ADC_MAX_VALUE_D))
	{
		return f32Voltage;
	}

	/* Calculate accurate ADC value */
	f32Voltage = -0.000000000000016 * pow((double_t)u16CapacitorADCValue, 4) + 0.000000000118171 * pow((double_t)u16CapacitorADCValue, 3) - 0.000000301211691 * pow((double_t)u16CapacitorADCValue, 2) + 0.001109019271794 * (double_t)u16CapacitorADCValue + 0.034143524634089;
	
	/* Multiply with divider constant */
	f32Voltage = f32Voltage * (double_t)DIVIDER_CONSTANT_D;
#if _DEBUG
	/* Debug console print */
	Serial.println("ADC value : " + String(u16CapacitorADCValue) + "  Accurate value : " + String(f32Voltage));
#endif

	return f32Voltage;
}
