#include "accelerometer.h"

struct accelerometer accelerometer_state = {0,0xFFFF,0,0};

void init_accelerometer(){
    sACCEL_Init();
    sACCEL_WriteReg(0x20, 0x83);
    uint8_t b = sACCEL_ReadReg(0x21);
    b |= 0x20;
    //Set to be big endian
    sACCEL_WriteReg(0x21, b);
    init_timer_with_interrupt();
}

void clear_accelerometer(){
    accelerometer_state.current_vibration=0x0;
    accelerometer_state.min=0xffff;
    accelerometer_state.max=0x0;
    accelerometer_state.sample_flag=0x0;
}

void TIM5_IRQHandler(){
    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        accelerometer_state.sample_flag = 1;
    }
}

uint8_t sample_accelerometer (){
    if (accelerometer_state.sample_flag == 1){
        accelerometer_state.sample_flag = 0;

        uint16_t current_val = read_accelerotmeter_Y();

        if (current_val > accelerometer_state.max) {
            accelerometer_state.max = current_val;
        }
        if (current_val < accelerometer_state.min){
            accelerometer_state.min = current_val;
        }
        uint16_t delta = accelerometer_state.max-accelerometer_state.min;
        if (0 <= delta && delta < SEISMIC_LOW_TRESHOLD){
            accelerometer_state.current_vibration = SEISMIC_NONE;
        } else if (SEISMIC_LOW_TRESHOLD <= delta && delta < SEISMIC_MEDIUM_TRESHOLD){
            accelerometer_state.current_vibration = SEISMIC_LOW;
        } else if (SEISMIC_MEDIUM_TRESHOLD <= delta && delta < SEISMIC_HIGH_TRESHOLD) {
            accelerometer_state.current_vibration = SEISMIC_MEDIUM;
        } else {
            accelerometer_state.current_vibration = SEISMIC_HIGH;
        }
        return 1;
    }
    return 0;
}

void init_timer_with_interrupt(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    
    //Enabeling timer interrupt
    NVIC_InitTypeDef blob;
    blob.NVIC_IRQChannel = TIM5_IRQn;
    blob.NVIC_IRQChannelPreemptionPriority = 8;
    blob.NVIC_IRQChannelSubPriority = 0;
    blob.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&blob);
    
    //Enabeling timer TIM_3
    TIM_TimeBaseInitTypeDef timer;
    
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_ClockDivision = TIM_CKD_DIV1;
    timer.TIM_Prescaler = 59;
    timer.TIM_Period = 1000*50;
    timer.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &timer);
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  
    TIM_Cmd(TIM5, ENABLE);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

uint16_t read_accelerotmeter_Y(){
    uint8_t highRetY = sACCEL_ReadReg(0x2A);
    uint8_t lowRetY = sACCEL_ReadReg(0x2B);
    uint16_t y = ((u16)(highRetY & 0x3F) << 8) | lowRetY;
    
    return y;
}

uint16_t read_accelerotmeter_X(){
    uint8_t highRetX = sACCEL_ReadReg(0x28);
    uint8_t lowRetX = sACCEL_ReadReg(0x29);
    uint16_t x = ((u16)(highRetX & 0x3F) << 8) | lowRetX;

    return x;
}

