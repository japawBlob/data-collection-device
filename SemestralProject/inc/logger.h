#ifndef __LOGGER_H
#define __LOGGER_H

#include "stm32f2xx_rtc.h"
#include "stm32_eval_i2c_ee.h"
#include <stdlib.h>
#include "stm32_eval.h"
#include "accelerometer.h"

#define start_address 0xB0
#define RECORDS_MAX_NUM 300

struct data_entry {
    // RTC_DateTypeDef date;
    // RTC_TimeTypeDef time;
    uint8_t time_stamp;
    int8_t temperature;
    uint8_t humidity;
    uint8_t vibrations;
};

extern struct data_entry current_entry;

void init_logger();

void append_mem_entry(struct data_entry);

struct data_entry* get_entries();

uint16_t get_number_of_records();

void store_record(struct data_entry de);

void store_record_at_address(struct data_entry de, uint16_t address);

struct data_entry get_record_at_address(uint16_t address);

void write_mem_entry(struct data_entry de, uint16_t address);

void send_data();

#endif