#ifndef SCAF_IOT_H
#define SCAF_IOT_H

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

#include "lwip/err.h"
#include "lwip/sys.h"
#include <iotc.h>
#include <iotc_jwt.h>

#include "globals.h"

/* Google IoT Paths Definitions */
#define DEVICE_PATH "projects/%s/locations/%s/registries/%s/devices/%s"
#define SUBSCRIBE_TOPIC_COMMAND "/devices/%s/commands/#"
#define SUBSCRIBE_TOPIC_CONFIG "/devices/%s/config"
#define PUBLISH_TOPIC_EVENT "/devices/%s/events"
#define PUBLISH_TOPIC_STATE "/devices/%s/state"
#define WEIGHT_MSG "{\"weight\":%.2f, \"timestamp\":%d%02d%02d%02d%02d}"
#define TEST_MSG "This is a test"

void iotc_init();
void mqtt_task(void *pvParameters);
void publish_telemetry_event(iotc_context_handle_t, iotc_timed_task_handle_t, void *);

#endif