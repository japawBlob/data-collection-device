
#ifndef __DHT11_H
#define __DHT11_H

#define INPUT 0
#define OUTPUT 1

#include "stm32_eval.h"

#define DHT11_GPIO GPIOB
#define DHT11_PIN GPIO_Pin_0
#define DHT11_TIM TIM2

struct DHT11_response {
    uint8_t integral_RH;
    uint8_t decimal_RH;
    uint8_t integral_T;
    uint8_t decimal_T;
    uint8_t checksum;
};



void setup_dht11();
struct DHT11_response read_DHT11();

#endif