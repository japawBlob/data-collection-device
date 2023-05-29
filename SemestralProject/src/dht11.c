#include "dht11.h"
#include "lcd_framebuffer.h"

static uint8_t DHT11_temperature = 0;
static uint8_t DHT11_humidity = 0;

static void set_dht11_pin(uint8_t mode){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    if (mode == INPUT) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(DHT11_GPIO, &GPIO_InitStructure);
    }
    if (mode == OUTPUT) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(DHT11_GPIO, &GPIO_InitStructure);
    }
}

static void set_timer(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_TimeBaseInitTypeDef timer;
    TIM_TimeBaseStructInit(&timer);
    
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_ClockDivision = TIM_CKD_DIV1;
    timer.TIM_Prescaler = 59;
    timer.TIM_Period = 1000000;
    timer.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(DHT11_TIM, &timer);
    
    TIM_ITConfig(DHT11_TIM, TIM_IT_Update, ENABLE);
    TIM_Cmd(DHT11_TIM, ENABLE);
}

void setup_dht11(){
    set_dht11_pin(OUTPUT);
    set_timer();
    TIM_SetCounter(TIM2,0);
    uint32_t end = TIM_GetCounter(TIM2);
    
}
#define RESPONSE 0
#define NO_RESPONSE 1
static uint8_t wait_for_response(uint8_t status){
    TIM_SetCounter(DHT11_TIM, 0);
    while(GPIO_ReadInputDataBit(DHT11_GPIO, DHT11_PIN) == status){
        if(TIM_GetCounter(DHT11_TIM) > 500){
            return NO_RESPONSE;
        }
    } 
    return RESPONSE;
}

static struct DHT11_response read_DHT11_2(){
    struct DHT11_response ret;
    
    set_dht11_pin(OUTPUT);
    GPIO_ResetBits(DHT11_GPIO, DHT11_PIN);
    TIM_GetCounter(DHT11_TIM);
    TIM_SetCounter(DHT11_TIM, 0);
    
    while(TIM_GetCounter(DHT11_TIM) < 18000);
    // my idea
    GPIO_SetBits(DHT11_GPIO, DHT11_PIN);
    //
    __disable_irq();
    TIM_SetCounter(DHT11_TIM, 0);
    set_dht11_pin(INPUT);
    
    if( wait_for_response(SET) == NO_RESPONSE ){
        __enable_irq();
        ret.checksum = 1;
        return ret;
    }
    
    if( wait_for_response(RESET) == NO_RESPONSE ){
        __enable_irq();
        ret.checksum = 2;
        return ret;
    }
    unsigned tim1 = TIM_GetCounter(DHT11_TIM);
    if( wait_for_response(SET) == NO_RESPONSE ){
        __enable_irq();
        ret.checksum = 3;
        return ret;
    }
    unsigned tim2 = TIM_GetCounter(DHT11_TIM);
    int i;
    uint8_t data [40];
    for (i = 0; i<40; i++){
        if( wait_for_response(RESET) == NO_RESPONSE ){
            __enable_irq();
            ret.checksum = 100+i;
            return ret;
        }
        if( wait_for_response(SET) == NO_RESPONSE ){
            __enable_irq();
            ret.checksum = 200+i;
            return ret;
        }
        tim1 = TIM_GetCounter(DHT11_TIM);
        uint8_t bitval = 0;
        if (tim1 > 20 && tim1 < 30){
            bitval = 0;
        }
        else if (tim1 > 60 && tim1 < 80){
            bitval = 1;
        }
        data[i] = bitval;
    }
    __enable_irq();
   
    
    for(i=0; i<8; i++){
       ret.integral_RH += data[i];
       ret.integral_RH = ret.integral_RH << 1;
    }
    for(i=8; i<16; i++){
       ret.decimal_RH += data[i];
       ret.decimal_RH = ret.decimal_RH << 1;
    }
    for(i=16; i<24; i++){
       ret.integral_T += data[i];
       ret.integral_T = ret.integral_T << 1;
    }
    for(i=24; i<32; i++){
       ret.decimal_T += data[i];
       ret.decimal_T = ret.decimal_T << 1;
    }
    for(i=32; i<40; i++){
       ret.checksum += data[i];
       ret.checksum = ret.checksum << 1;
    }
    
    
    
    
    return ret;
}

struct DHT11_response read_DHT11(){

    uint8_t bits[5];
    int ch;
    for (ch=0; ch< 5; ch++) bits[ch] = 0;
    uint8_t cnt = 7;
    uint8_t idx = 0;
    struct DHT11_response ret;
    
    set_dht11_pin(OUTPUT);
    GPIO_ResetBits(DHT11_GPIO, DHT11_PIN);
    TIM_GetCounter(DHT11_TIM);
    TIM_SetCounter(DHT11_TIM, 0);
    
    while(TIM_GetCounter(DHT11_TIM) < 18000);
    // my idea
    GPIO_SetBits(DHT11_GPIO, DHT11_PIN);
    //
    __disable_irq();
    TIM_SetCounter(DHT11_TIM, 0);
    set_dht11_pin(INPUT);
    
    if( wait_for_response(SET) == NO_RESPONSE ){
        __enable_irq();
        ret.checksum = 10;
        return ret;
    }
    
    if( wait_for_response(RESET) == NO_RESPONSE ){
        __enable_irq();
        ret.checksum = 2;
        return ret;
    }
    unsigned tim1 = TIM_GetCounter(DHT11_TIM);
    if( wait_for_response(SET) == NO_RESPONSE ){
        __enable_irq();
        ret.checksum = 3;
        return ret;
    }
    unsigned tim2 = TIM_GetCounter(DHT11_TIM);
    int i;
    uint8_t data [40];
    for (i = 0; i<40; i++){
        if( wait_for_response(RESET) == NO_RESPONSE ){
            __enable_irq();
            ret.checksum = 100+i;
            return ret;
        }
        if( wait_for_response(SET) == NO_RESPONSE ){
            __enable_irq();
            ret.checksum = 200+i;
            return ret;
        }
        tim1 = TIM_GetCounter(DHT11_TIM);
        if(tim1 > 50) bits[idx] |= (1 << cnt);
        if (cnt == 0)
        {
            cnt = 7;   
            idx++;      
        }else{
            cnt--;
        } 
    }
    __enable_irq();
    
    ret.integral_RH = bits[0];
    ret.decimal_RH = bits[1];
    ret.integral_T = bits[2];
    ret.decimal_T = bits[3];
    ret.checksum = bits[4];
    
    DHT11_temperature = bits[2];
    DHT11_humidity = bits[0];

    if(ret.decimal_RH+ret.decimal_T+ret.integral_RH+ret.integral_T != ret.checksum) {
        
    }
     
    return ret;
}