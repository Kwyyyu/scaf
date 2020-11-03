#include "iot.h"
#include "motor.h"
#include "weight.h"
#include "globals.h"
#include "pir.h"
#include "lcd.h"
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "jsmn.h"
#include <time.h>
#include "lwip/apps/sntp.h"
#include "driver/gpio.h"

void main_task(void * pvParameters);