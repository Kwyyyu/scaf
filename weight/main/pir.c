/*
 * pir.c
 *
 *  Created on: Sep 29, 2020
 *      Author: kaiwenyu
 */


#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"

#define pir_read_pin 34

void app_main(){
	int flag = 0;
	printf("PIR sensor Started!\n");
	while(1){
		gpio_set_direction(pir_read_pin, GPIO_MODE_INPUT);
		int state = gpio_get_level(pir_read_pin);
		if (state == 1 && flag == 0){
			flag = 1;
			printf("PIR sensor Triggered!\n");
		}
		else if (state == 0 && flag == 1){
			flag = 0;
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}
