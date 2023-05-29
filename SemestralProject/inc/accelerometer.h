#ifndef __ACCELEROMETER_H
#define __ACCELEROMETER_H

#include "stm32_eval_spi_accel.h"

#define SEISMIC_LOW_TRESHOLD 5
#define SEISMIC_MEDIUM_TRESHOLD 20
#define SEISMIC_HIGH_TRESHOLD 100

enum {SEISMIC_NONE, SEISMIC_LOW, SEISMIC_MEDIUM, SEISMIC_HIGH};

struct accelerometer {
    uint16_t current_vibration;
    uint16_t min;
    uint16_t max;
    uint8_t sample_flag;
};

extern struct accelerometer accelerometer_state;

void init_accelerometer();

void clear_accelerometer();

uint16_t read_accelerotmeter_Y();

uint16_t read_accelerotmeter_X();

uint8_t sample_accelerometer();





#endif