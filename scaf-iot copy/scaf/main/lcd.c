#include "lcd.h"

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

void lcd_init() {
    ESP_LOGI(TAG, "Initializing SPIFFS");
    
    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };

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
}

void lcd_task(void * pvParameters) {
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
	double weight_prev;
	uint16_t color;
	lcdFillScreen(&dev, WHITE);
	color = BLACK;
	lcdDrawRoundRect(&dev, 5, 5, (width-5-1), (height-5-1), 10, color);
	lcdDrawFillCircle(&dev, 130,  220, 80, LIGHT_BROWN);

	unsigned char string[40];
    unsigned char wgt[40];
    unsigned char next[40];
    unsigned char tm[40];

    unsigned char wgt_prev[40];
    unsigned char next_prev[40];
    unsigned char tm_prev[40];

    time_t now = 0;
    struct tm timeinfo = {0};

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

	ESP_LOGI(TAG, "In LCD Task initial");


	// update time
	time(&now);
	localtime_r(&now, &timeinfo);
	sprintf((char *)tm, TIMESTAMP, timeinfo.tm_year+1900,
						timeinfo.tm_mon+1,
						timeinfo.tm_mday,
						timeinfo.tm_hour,
						timeinfo.tm_min);
	printf("TIMESTAMP: %s\n", tm);
	lcdDrawString(&dev, fx24G, 35, 295, tm, BLACK);

	// update weight
	if(weight<0) {weight = 0.0;}
	weight_prev = weight;
	sprintf((char *)wgt, "%.1lfg", weight);
	lcdDrawString(&dev, fx32G, 170, 260, wgt, WHITE);

	//update next meal plan
	sprintf((char *)next, "%02d:%02d", scheduled_time.tm_hour, scheduled_time.tm_min);
	lcdDrawString(&dev, fx32G, 190, 105, next, BLACK);

	WAIT;

	while(1) {
        ESP_LOGI(TAG, "In LCD Task");

		// update time
        time(&now);
        localtime_r(&now, &timeinfo);
        strcpy((char *)tm_prev, (char *)tm);
        sprintf((char *)tm, TIMESTAMP, timeinfo.tm_year+1900,
                            timeinfo.tm_mon+1,
                            timeinfo.tm_mday, 
                            timeinfo.tm_hour, 
                            timeinfo.tm_min);
        printf("TIMESTAMP: %s\n", tm);
        if (strcmp((char *)tm_prev, (char *)tm) != 0){
        	lcdDrawString(&dev, fx24G, 35, 295, tm_prev, WHITE);
        	lcdDrawString(&dev, fx24G, 35, 295, tm, BLACK);
        }


		// update weight
        if(weight<0) {weight = 0.0;}
        if (abs(weight_prev-weight) > 0.3){
        	strcpy((char *)wgt_prev, (char *)wgt);
        	sprintf((char *)wgt, "%.1lfg", weight);
        	weight_prev = weight;
			lcdDrawString(&dev, fx32G, 170, 260, wgt_prev, LIGHT_BROWN);
			lcdDrawString(&dev, fx32G, 170, 260, wgt, WHITE);
        }


		//update next meal plan
		strcpy((char *)next_prev, (char *)next);
	    sprintf((char *)next, "%02d:%02d", scheduled_time.tm_hour, scheduled_time.tm_min);
	    if (strcmp((char *)next_prev, (char *)next) != 0){
	    	lcdDrawString(&dev, fx32G, 190, 105, next_prev, WHITE);
	    	lcdDrawString(&dev, fx32G, 190, 105, next, BLACK);
	    }

		WAIT;
	} // end while

	// never reach
	while (1) {
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}
