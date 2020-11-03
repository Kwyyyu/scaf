#include "motor.h"

void motor_init()
{
    gpio_set_direction(MOTOR_EN, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_DIR, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_STEP, GPIO_MODE_OUTPUT);

    gpio_set_level(MOTOR_EN, 1);
    gpio_set_level(MOTOR_DIR, 0);
    gpio_set_level(MOTOR_STEP, 0);
}

void rotate(int portion)
{
    int delay = 60 * 1000 / NUM_STEP / RPM / 2;
    int steps = portion * PORTION_STEP;

    gpio_set_level(MOTOR_EN, 0);
    vTaskDelay(50 / portTICK_PERIOD_MS);

    while(steps) {
     /* Send pulse every delay to rotate motor one step */
     gpio_set_level(MOTOR_STEP, 0);
     vTaskDelay(delay / portTICK_PERIOD_MS);
     gpio_set_level(MOTOR_STEP, 0);
     vTaskDelay(delay / portTICK_PERIOD_MS);
     steps--;
    }

    gpio_set_level(MOTOR_EN, 1);

}
