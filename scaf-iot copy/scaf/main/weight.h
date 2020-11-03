#ifndef SCAF_WEIGHT_H
#define SCAF_WEIGHT_H

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "hx711.h"
#include "esp_log.h"
#include "globals.h"

#define PD_SCK_GPIO 33
#define DOUT_GPIO   32

void weight_task(void *pvParameters);

#endif