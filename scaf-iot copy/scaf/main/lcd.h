#ifndef SCAF_LCD_H
#define SCAF_LCD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_heap_caps.h"
#include <time.h>
#include "fontx.h"
#include "ili9340.h"
#include "globals.h"

#define	INTERVAL		2000
#define WAIT	vTaskDelay(INTERVAL / portTICK_PERIOD_MS)
#define CONFIG_WIDTH  240
#define CONFIG_HEIGHT 320
#define CONFIG_CS_GPIO 21
#define CONFIG_DC_GPIO 27
#define CONFIG_RESET_GPIO 25
#define CONFIG_BL_GPIO 26
#define CONFIG_OFFSETX 0
#define CONFIG_OFFSETY 0

#define LIGHT_BROWN 0x93CF
#define DARK_BROWN 0x7B0B

#define TIMESTAMP "%d/%02d/%02d %02d:%02d"

void lcd_init();
void lcd_task(void *pvParameters);

#endif
