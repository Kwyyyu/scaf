#ifndef SCAF_PIR_H
#define SCAF_PIR_H

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "iot.h"
#include "esp_log.h"

#define PIR_READ_PIN 34

void pir_init();
void pir_task(void *);

#endif