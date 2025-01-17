#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "hx711.h"

#define PD_SCK_GPIO 33
#define DOUT_GPIO   32

void test(void *pvParameters)
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
        	r = hx711_read_data(&dev, &offset);
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

    // read from device
    while (1)
    {
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


        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(test, "test", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
}
