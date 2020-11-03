#include "main.h"

int portion;
struct tm scheduled_time;
double weight;

void misc_init(void)
{
    portion = 0;
    scheduled_time.tm_year = 0;
    scheduled_time.tm_mon = 0;
    scheduled_time.tm_mday = 0;
    scheduled_time.tm_hour = 0;
    scheduled_time.tm_min = 0;
    weight = 0;
}

void main_task( void * pvParameters )
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 60000 / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();  

    for( ;; )
    {
        time_t now = 0;
        struct tm timeinfo = {0};
        time(&now);
        localtime_r(&now, &timeinfo);
        ESP_LOGI("APP", "In main task");
        if (scheduled_time.tm_year == timeinfo.tm_year &&
            scheduled_time.tm_mon == timeinfo.tm_mon &&
            scheduled_time.tm_mday == timeinfo.tm_mday &&
            scheduled_time.tm_hour == timeinfo.tm_hour &&
            scheduled_time.tm_min == timeinfo.tm_min)
        {
        	printf("Start rotating.");
            rotate(portion);
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


void app_main()
{
    /* Initialize nvs */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Initializations */
    misc_init();
    iotc_init();
    motor_init();
    lcd_init();
    pir_init();

    /* Tasks */
    xTaskCreate(&lcd_task, "lcd_task", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
    xTaskCreate(&mqtt_task, "mqtt_task", 8192, NULL, 5, NULL);
    xTaskCreate(&weight_task, "weight_task", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
    xTaskCreate(&pir_task, "pir_task", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
    xTaskCreate(&main_task, "main_task", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
    // vTaskStartScheduler();
    
}
