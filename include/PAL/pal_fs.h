// pal_FileSystem.h

#ifndef _PAL_FILESYSTEM_h
#define _PAL_FILESYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"
#include "pal_defines.h"
#include "ArduinoJson.h"

#define CONFIG_FILE_PATH_D					("/cfg.json")
#define MAX_JSON_FILESIZE_D					(1024u)
#define EMPTY_FILESIZE_D					(0u)

/* Function used to initialize File system on ESP-32 hardware */
extern uint16_t pal_fs_u16FileSystemInitialization(void);
/* Function used to set config file data */
extern void pal_fs_vSetReadedData(String sDataToStore);
/* Function used to get config file data */
extern String pal_fs_sGetReadedData(void);
/*Function used to read JSON file to get network credencials*/
extern uint16_t pal_fs_u16GetCredentials(const char* path, Config_T* pCredentials);

#endif

