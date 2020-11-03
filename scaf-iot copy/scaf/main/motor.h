#ifndef SCAF_MOTOR_H
#define SCAF_MOTOR_H

#include "globals.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define MOTOR_EN 12
#define MOTOR_STEP 15
#define MOTOR_DIR 14
#define NUM_STEP 200
#define RPM 5
#define PORTION_STEP 33

void motor_init();
void rotate(int);

#endif
