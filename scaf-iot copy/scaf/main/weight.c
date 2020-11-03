#include "weight.h"

void weight_task(void *pvParameters)
{
    hx711_t dev = {
        .dout = DOUT_GPIO,
        .pd_sck = PD_SCK_GPIO,
        .gain = HX711_GAIN_A_64
    };

    int32_t offset;

    // initialize device
    while (1)
    {
        esp_err_t r = hx711_init(&dev);
        if (r == ESP_OK)
        {
        	r = hx711_wait(&dev, 200);
//        	r = hx711_read_data(&dev, &offset);
        	offset = 85645;
        	printf("Offset: %d\n", offset);
            break;
        }
        printf("Could not initialize HX711: %d (%s)\n", r, esp_err_to_name(r));
        vTaskDelay(500 / portTICK_PERIOD_MS);

		if (r != ESP_OK)
		{
			printf("Could not read data: %d (%s)\n", r, esp_err_to_name(r));
			continue;
		}
    }

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 1000 / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();  

    // read from device
    while (1)
    {
        ESP_LOGI("APP", "In WEight Task");
        esp_err_t r = hx711_wait(&dev, 500);
        if (r != ESP_OK)
        {
            printf("Device not found: %d (%s)\n", r, esp_err_to_name(r));
            continue;
        }

        int32_t data;
        r = hx711_read_data(&dev, &data);
        if (r != ESP_OK)
        {
            printf("Could not read data: %d (%s)\n", r, esp_err_to_name(r));
            continue;
        }

        printf("Scaled data: %f grams\n", (data-offset)/222.825);
        weight = (data-offset)/222.825;
        
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
