/* SPIFFS filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

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
#include "fontx.h"
#include "ili9340.h"

#define	INTERVAL		1000
#define WAIT	vTaskDelay(INTERVAL)

static const char *TAG = "ILI9340";

static void SPIFFS_Directory(char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		ESP_LOGI(__FUNCTION__,"d_name=%s d_ino=%d d_type=%x", pe->d_name,pe->d_ino, pe->d_type);
	}
	closedir(dir);
}

#define CONFIG_WIDTH  240
#define CONFIG_HEIGHT 320
#define CONFIG_CS_GPIO 14
#define CONFIG_DC_GPIO 27
#define CONFIG_RESET_GPIO 33
#define CONFIG_BL_GPIO 32
#define CONFIG_OFFSETX 0
#define CONFIG_OFFSETY 0

#define LIGHT_BROWN 0x93CF
#define DARK_BROWN 0x7B0B


void ILI9341(void *pvParameters)
{
	TFT_t dev;
	uint16_t model = 0x9340;
	spi_master_init(&dev, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
	lcdInit(&dev, model, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);

	FontxFile fx16G[2];
	FontxFile fx24G[2];
	FontxFile fx32G[2];
	InitFontx(fx16G,"/spiffs/ILGH16XB.FNT",""); // 8x16Dot Gothic
	InitFontx(fx24G,"/spiffs/ILGH24XB.FNT",""); // 12x24Dot Gothic
	InitFontx(fx32G,"/spiffs/ILGH32XB.FNT",""); // 16x32Dot Gothic

	int width = CONFIG_WIDTH;  //240
	int height = CONFIG_HEIGHT;  //320
	uint16_t color;
	lcdFillScreen(&dev, WHITE);
	color = BLACK;
	lcdDrawRoundRect(&dev, 5, 5, (width-5-1), (height-5-1), 10, color);
	lcdDrawFillCircle(&dev, 130,  220, 80, LIGHT_BROWN);

	uint8_t time[40];
	uint8_t weight[40];
	uint8_t string[40];
	uint8_t next[40];

	lcdSetFontDirection(&dev, 3);
	strcpy((char *)string, "Current");
	lcdDrawString(&dev, fx24G, 95, 260, string, WHITE);
	strcpy((char *)string, "weight:");
	lcdDrawString(&dev, fx24G, 115, 260, string, WHITE);

	strcpy((char *)string, "Time");
	lcdDrawString(&dev, fx24G, 90, 90, string, DARK_BROWN);
	strcpy((char *)string, "for");
	lcdDrawString(&dev, fx16G, 110, 80, string, DARK_BROWN);
	strcpy((char *)string, "Next Meal");
	lcdDrawString(&dev, fx24G, 135, 120, string, DARK_BROWN);

	strcpy((char *)time, "2020/10/21 22:39");
	int cnt = 0;

	while(1) {

		// update time
		if (cnt == 1)
		{
			lcdDrawString(&dev, fx24G, 35, 295, time, WHITE);
			strcpy((char *)time, "2020/10/21 22:40");
			lcdDrawString(&dev, fx24G, 35, 295, time, BLACK);
			strcpy((char *)time, "2020/10/21 22:41");
		}
		else{
			lcdDrawString(&dev, fx24G, 35, 295, time, BLACK);
		}


		// update weight
		strcpy((char *)weight, "105.7g");
		lcdDrawString(&dev, fx32G, 170, 270, weight, WHITE);

		//update next meal plan
	    strcpy((char *)next, "07:30");
	    lcdDrawString(&dev, fx32G, 190, 100, next, BLACK);

		WAIT;
		cnt ++;


	} // end while

	// never reach
	while (1) {
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}


void app_main(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");
    
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };
    
    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    SPIFFS_Directory("/spiffs/");

    xTaskCreate(ILI9341, "ILI9340", 1024*6, NULL, 2, NULL);

}
