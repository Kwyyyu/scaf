/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/ledc.h"
#include "esp_err.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO_0 12
#define BLINK_GPIO_1 13
#define BLINK_GPIO_2 14
#define BLINK_GPIO_3 15

void app_main()
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
	int ch;
	/* Select GPIO and set the GPIO as a push/pull output */
	for(ch = 12; ch<16;ch++){
		gpio_pad_select_gpio(ch);
		 gpio_set_direction(ch, GPIO_MODE_OUTPUT);
	}
//    gpio_pad_select_gpio(BLINK_GPIO);

//    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
	printf("Turning off the LED\n");

	for(ch = 12; ch<16;ch++){
		gpio_set_level(ch, 0);
		}
    vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
	printf("Turning on the LED\n");
	for(ch = 12; ch<16;ch++){
		gpio_set_level(ch, 1);
		}
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
