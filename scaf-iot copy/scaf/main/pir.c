#include "pir.h"

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = PIR_READ_PIN;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void pir_task( void * pvParameters )
{
    uint32_t io_num;
    for( ;; )
    {
        ESP_LOGI("APP", "In PIR Task");
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            ESP_LOGI("APP", "ISR TRIGGERED");
            if (!gpio_get_level(PIR_READ_PIN))
            {
                publish_telemetry_event(iotc_context, 0, NULL);
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void pir_init()
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = 1ULL<<PIR_READ_PIN;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(PIR_READ_PIN, gpio_isr_handler, (void*) PIR_READ_PIN);
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
}