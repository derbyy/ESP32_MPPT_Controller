#include "PAL/pal_fs.h"


/******************* Module static variables ************************/
static String pal_fs_sConfigFileData;
/********************************************************************/


/********************************************************************
* Parameters		:       void                                    *
* Return value		:		uint16_t								*
* Description		:		Function used to initialize File		*
*							system on ESP-32 hardware				*
/********************************************************************/
uint16_t pal_fs_u16FileSystemInitialization(void)
{
	uint16_t u16Result;

	/* Initialize file system */
	if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED_D))
	{
#if _DEBUG
		Serial.println("File system initialization failed !!!");
#endif
		/* Set error if file system initialization failure */
		u16Result = ERR_FILESYSTEM_INIT_FAILED_D;
	}
	else
	{
#if _DEBUG
		Serial.println("File system initialized.");
#endif
		u16Result = ERR_NO_ERROR_D;
	}

	return u16Result;
}

/********************************************************************
* Parameters		:       String sDataToStore                     *
* Return value		:		void									*
* Description		:		Function used to set config file data	*
/********************************************************************/
void pal_fs_vSetReadedData(String sDataToStore)
{
	pal_fs_sConfigFileData = sDataToStore;
}

/********************************************************************
* Parameters		:       void					                *
* Return value		:		String									*
* Description		:		Function used to get config file data	*
/********************************************************************/
String pal_fs_sGetReadedData(void)
{
	return pal_fs_sConfigFileData;
}

/********************************************************************
* Parameters		:       void									*
* Return value		:		uint16_t								*
* Description		:		Function used to read JSON file to get  *
*							network credencials						*
/********************************************************************/
uint16_t pal_fs_u16GetCredentials(const char* path, Config_T* pCredentials)
{
	uint16_t u16Result;
	size_t u32FileSize;

	/* Open file at specific location */
	File pFile = SPIFFS.open(path, "r");

	/* Set file read error if file or directory doesnt exists */
	if (!pFile || pFile.isDirectory())
	{
#if _DEBUG
		Serial.println("Failed to open file for reading !!!");
#endif
		u16Result = ERR_FILE_OPEN_ERROR_D;
	}
	else
	{
		/* Get size of file stored in filesystem */
		u32FileSize = pFile.size();

		/* Check filesize */
		if (u32FileSize > (uint32_t)MAX_JSON_FILESIZE_D)
		{
			u16Result = ERR_FILE_TOO_BIG_D;
		}
		else if (u32FileSize == (uint32_t)EMPTY_FILESIZE_D)
		{
#if _DEBUG
			Serial.println("File is empty !!!");
#endif
			u16Result = ERR_CONFIG_FILE_EMPTY_D;
		}
		else
		{
#if _DEBUG
			Serial.printf("File size is : %d bytes\r\n", u32FileSize);
#endif
			u16Result = ERR_NO_ERROR_D;
		}

		/* Allocate buffer for JSON data storing */
		char* pchJsonData = new char[u32FileSize];

#if _DEBUG
		Serial.printf("Reading file: %s\r\n", path);
#endif
		/* Read file at specific location */
		pFile.readBytes(pchJsonData, u32FileSize);

		/* Create JSON document object */
		StaticJsonDocument<MAX_JSON_FILESIZE_D> jsonDocument;

		/* Deserialize JSON document */
		DeserializationError deserializeError = deserializeJson(jsonDocument, pchJsonData);

		/* Check deserialization result */
		if (deserializeError)
		{
#if _DEBUG
			Serial.println(" JSON - Error occured : " + String(deserializeError.c_str()));
#endif
			u16Result = ERR_JSON_DESERIALIZATION_D;
		}
		else
		{
			/* Parse credential data */
			const char* ssid = jsonDocument["ssid"];
			const char* password = jsonDocument["password"];

			/* Store credentials into structure */
			pCredentials->strSsid = String(ssid);
			pCredentials->strPassword = String(password);

#if _DEBUG
			Serial.println("Data : " + pCredentials->strSsid + ", " + pCredentials->strPassword);
#endif
		}

		/* Free allocated buffer */
		delete[] pchJsonData;
	}

	/* Close file */
	pFile.close();

	return u16Result;
}

/********************************************************************
* Parameters		:       const char*							    *
*							uint8_t									*
* Return value		:		void								    *
* Description		:		Function used to list directories in 	*
* 							SPIFFS									*
/********************************************************************/
void listDir(const char* dirname, uint8_t levels) 
{
#if _DEBUG
	Serial.printf("Listing directory: %s\r\n", dirname);
#endif

	File root = SPIFFS.open(dirname);
	if (!root) 
	{
#if _DEBUG
		Serial.println("- failed to open directory");
#endif		
		return;
	}
	if (!root.isDirectory()) 
	{
#if _DEBUG
		Serial.println(" - not a directory");
#endif		
		return;
	}

	File file = root.openNextFile();
	while (file) 
	{
		if (file.isDirectory()) 
		{
#if _DEBUG
			Serial.print("  DIR : ");
			Serial.println(file.name());
#endif			
			if (levels) 
			{
				listDir(file.name(), levels - 1);
			}
		}
		else 
		{
#if _DEBUG			
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("\tSIZE: ");
			Serial.println(file.size());
#endif			
		}
		file = root.openNextFile();
	}
}

/********************************************************************
* Parameters		:       FS&										*
*							const char*							    *
*							const char*								*
* Return value		:		void								    *
* Description		:		Function used to write text into the 	*
*							file									*
/********************************************************************/
void writeFile(fs::FS& fs, const char* path, const char* message) 
{
#if _DEBUG	
	Serial.printf("Writing file: %s\r\n", path);
#endif

	File file = fs.open(path, FILE_WRITE);
	if (!file) 
	{
#if _DEBUG
		Serial.println("- failed to open file for writing");
#endif
		return;
	}	
	if (file.print(message)) 
	{
#if _DEBUG
		Serial.println("- file written");
#endif
	}
	else 
	{
#if _DEBUG
		Serial.println("- frite failed");
#endif
	}
}

/********************************************************************
* Parameters		:       FS&										*
*							const char*							    *
*							const char*								*
* Return value		:		void								    *
* Description		:		Function used to append text into the 	*
*							file									*
/********************************************************************/
void appendFile(fs::FS& fs, const char* path, const char* message) 
{
#if _DEBUG	
	Serial.printf("Appending to file: %s\r\n", path);
#endif

	File file = fs.open(path, FILE_APPEND);
	if (!file) 
	{
#if _DEBUG		
		Serial.println("- failed to open file for appending");
#endif
		return;
	}
	if (file.print(message)) 
	{
#if _DEBUG
		Serial.println("- message appended");
#endif
	}
	else 
	{
#if _DEBUG
		Serial.println("- append failed");
#endif
	}
}

/********************************************************************
* Parameters		:       FS&										*
*							const char*							    *
*							const char*								*
* Return value		:		void								    *
* Description		:		Function used to rename file		 	*
/********************************************************************/
void renameFile(fs::FS& fs, const char* path1, const char* path2) 
{
#if _DEBUG
	Serial.printf("Renaming file %s to %s\r\n", path1, path2);
#endif
	if (fs.rename(path1, path2))
	{
#if _DEBUG		
		Serial.println("- file renamed");
#endif
	}
	else 
	{
#if _DEBUG
		Serial.println("- rename failed");
#endif
	}
}

/********************************************************************
* Parameters		:       FS&										*
*							const char*							    *
* Return value		:		void								    *
* Description		:		Function used to delete file		 	*
/********************************************************************/
void deleteFile(fs::FS& fs, const char* path) 
{
#if _DEBUG	
	Serial.printf("Deleting file: %s\r\n", path);
#endif

	if (fs.remove(path)) 
	{
#if _DEBUG
		Serial.println("- file deleted");
#endif
	}
	else 
	{
#if _DEBUG
		Serial.println("- delete failed");
#endif
	}
}