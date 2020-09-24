#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define PUL_GPIO 15
#define DIR_GPIO 32
#define ENA_GPIO 14
#define NUM_STEP 200
#define RPM 1
#define PORTION_STEP 33

void app_main()
{
	/* Configure GPIOs to output mode */
    gpio_pad_select_gpio(PUL_GPIO);
    gpio_set_direction(PUL_GPIO, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(DIR_GPIO);
    gpio_set_direction(DIR_GPIO, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(ENA_GPIO);
    gpio_set_direction(ENA_GPIO, GPIO_MODE_OUTPUT);

    /* Initialize GPIO */
    gpio_set_level(PUL_GPIO, 0);
    gpio_set_level(DIR_GPIO, 1);
    //gpio_set_level(ENA_GPIO, 1);

    int portion = 1; // need to fetch from cloud
    int steps = portion * PORTION_STEP;
    int delay = 60 * 1000 / NUM_STEP / RPM / 2; // half delay in ms

    while(steps) {
    	/* Send pulse every delay to rotate motor one step */
    	gpio_set_level(PUL_GPIO, 1);
    	vTaskDelay(delay / portTICK_PERIOD_MS);
    	gpio_set_level(PUL_GPIO, 0);
    	vTaskDelay(delay / portTICK_PERIOD_MS);
    	steps--;
    }
}
