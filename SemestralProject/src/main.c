/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    07-October-2011
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "stm32_eval.h"
#include "stm32f2xx_rtc.h"
#include "stm32_eval_spi_accel.h"
#include "lcd_framebuffer.h"
#include "stm32_eval_i2c_ee.h"
#include "dht11.h"
#include "stm32f2xx_pwr.h"
#include "accelerometer.h"
#include "logger.h"
#include "scene_handler.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_MAX_CHARS (int)(LCD_PIXEL_WIDTH/ LCD_FONT_WIDTH)
#define ENCODER_COUNTER_ZERO (0xFFFF/2)
#define ENCODER_POSITION (uint16_t)(TIM4->CNT)/4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void delay(vu32 nCount);
void init_IO();
void Button_Init(void);
void init_rtc();
void init_accelerometer();
void InitADC(void);
/* Private functions ---------------------------------------------------------*/

/**
 * Main program.
 * @return Nothing.
 */
uint8_t buf[100];

static void used_functions(){
    // eeprom:
    sEE_Init();
    sEE_ReadBuffer(buf, 0x200, 1);
    sEE_WriteBuffer(buf, 0x200, 1);
    
    // dht11
    setup_dht11();
    struct DHT11_response ret = read_DHT11();
    uint8_t temperature = ret.integral_T;
    uint8_t humidity = ret.integral_RH;
    
    // rtc
    //....
    
//    RTC_WriteBackupRegister();
//    RTC_ReadBackupRegister();
    
    RTC_DateTypeDef date;
    RTC_GetDate(0, &date);
    RTC_TimeTypeDef time;
    RTC_GetTime(0, &time);
    
    // accelerometer
    //....
    
    // ADC
    //....
    
    // LCD controll and screens
    //....
    
    // LCD scroll?
}

int main(void) {
    // sEE_WriteBuffer(0, 0x0, sizeof(size_t));
    init_IO();
    
    // uint16_t b = 0;
    //sEE_WriteBuffer(&b, start_address, sizeof(uint16_t));

//    struct data_entry blob = {
//        4, 5, 6, 55
//    };  
//    append_mem_entry(blob);
    
    iprintf("running\n");
//    LCD_DisplayStringLine(Line0, "Hello.");
//    LCD_DisplayStringLine(Line6, "huhuhu");  
//    LCD_DisplayStringLine(Line6, "heheheh");
    unsigned counter = 0;
//    uint16_t num = get_number_of_records();
//    struct data_entry * entries = get_entries();
//    int i;
//    iprintf("%d\n", get_number_of_records());
//    for(i=0; i<num; i++){
//        char buff [100];
//        sprintf(buff, "%d: %d  %d    %d    %d\n", i, entries[i].time_stamp, entries[i].temperature, entries[i].humidity, entries[i].vibrations);
//        iprintf("%s", buff);
//    }
//    iprintf("done\n");
//    struct data_entry buffrator [10];
//    uint16_t size = 10*sizeof(struct data_entry);
//    sEE_ReadBuffer(buffrator, start_address+sizeof(uint16_t), &size);
//    for(i=0; i<10; i++){
//        char buff [100];
//        sprintf(buff, "%d: %d  %d   %d    %d\n", i, buffrator[i].time_stamp, buffrator[i].temperature, buffrator[i].humidity, buffrator[i].vibrations);
//        iprintf("%s", buff);
//    }
    // set_number_of_records(0);
    unsigned report_count = 0;
    set_home_hub();
    while(1){
        // Handle butttons
        // Change scene
        
        // Handle Accelerometer
        //      Sample
        counter += sample_accelerometer();
        
        // Every second (or so)
        if (counter >= 20){
            
            /// Write temp
            struct data_entry new_entry;
            struct DHT11_response resp = read_DHT11();
            /// Write temp
            new_entry.temperature = resp.integral_T; //USE TERMISTOR IF TIME
            /// Write humidity
            new_entry.humidity = resp.integral_RH;
            /// Write Seismic
            new_entry.vibrations = accelerometer_state.current_vibration;
            // iprintf("%u %u %u\n\r", accelerometer_state.max, accelerometer_state.min, new_entry.vibrations);
            clear_accelerometer();
            /// Read time
            RTC_TimeTypeDef time;
            RTC_GetTime(0, &time);
            new_entry.time_stamp = time.RTC_Seconds;
            append_mem_entry(new_entry);
            counter = 0;
            update_scene();
            report_count++;
        }
        handle_user_input();
        
//        ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);
//        ADC_SoftwareStartConv(ADC1);
//        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

        // Read the converted value from the ADC data register
        // iprintf("%u  %u\n", sizeof(struct data_entry), sizeof(int));
        // delay(0xFFF);
//        struct DHT11_response resp = read_DHT11();
//        iprintf("OUTSICE %d %d %d %d %d\n ", resp.integral_T, resp.integral_RH, resp.decimal_T, resp.decimal_RH, resp.checksum);
//            
    }
}

void handle_user_input(){
    if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET){
        char user_input;
        user_input = USART_ReceiveData(USART3); // Collect Char
        if (user_input == 0xcc){
            set_number_of_records(0);
            current_line = 0;
            if (current_scene == DATA_SCREEN){
                set_data();
            }
        }
        if (user_input == 0xdd){
            send_data();
        }
    }
    if(STM_EVAL_PBGetState(BUTTON_SW2) == 0){
        if(current_scene == HOME_HUB){
            set_thermostat();
        } else 
        if(current_scene == SET_THERMOSTAT){
            sent_temp();
        } else 
        if(current_scene == DATA_SCREEN){
            send_data();
        }
        again:
        while(STM_EVAL_PBGetState(BUTTON_SW2) == 0);
        delay(0xf);
        if(STM_EVAL_PBGetState(BUTTON_SW2) == 0){
            goto again;
        }
    }
    if(STM_EVAL_PBGetState(BUTTON_SW1) == 0){ 
        if(current_scene == HOME_HUB){
            set_data();
        } else 
        if(current_scene == SET_THERMOSTAT){
            set_home_hub();
        } else 
        if(current_scene == DATA_SCREEN){
            set_home_hub();
        }
        again2:
        while(STM_EVAL_PBGetState(BUTTON_SW1) == 0);
        delay(0xfff);
        if(STM_EVAL_PBGetState(BUTTON_SW1) == 0){
            goto again2;
        }
        delay(0xfff);
    }
    static uint16_t last_pos = ENCODER_COUNTER_ZERO;
    if ( last_pos != ENCODER_POSITION){
        if(current_scene == SET_THERMOSTAT){
            if(ENCODER_POSITION > last_pos){
                if(desired_temp<0xff) {
                    desired_temp++;
                    set_thermostat();
                }
            } else {
                if(desired_temp>0) {
                    desired_temp--;
                    set_thermostat();
                }
            }
        } else 
        if(current_scene == DATA_SCREEN){
            if(ENCODER_POSITION > last_pos){
                if(current_line<get_number_of_records()){
                    current_line++;
                    set_data();
                }
            } else {
                if(current_line>0){
                    current_line--;
                    set_data();
                } else {
                    
                }
            }
        }
        // iprintf("%d\n", ENCODER_POSITION);
        last_pos = ENCODER_POSITION;
        // TIM4->CNT=ENCODER_COUNTER_ZERO;
        delay(0xfff);
    }
}

void init_IO(){
    ComPort_Init(COM2); // J15 - UART3
    /* Configure stdin,  stdout, stderr */
    ComPort_ConfigStdio(COM2, COM2, COM2);
    Button_Init();
    setup_dht11();
    init_rtc();
    sEE_Init();
    LCD_Init();
    LCD_SetBacklight(100);
    LCD_Clear();
    delay(0xFFFFF);
    LCD_Clear();
    init_accelerometer();
    init_logger();
    init_encoder_timer();
}

void init_rtc(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
    RTC_WriteProtectionCmd(DISABLE);
    RCC_LSICmd(ENABLE);
    
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) {}
    
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
    
    
    RTC_InitTypeDef rtc_init;
    RTC_StructInit(&rtc_init);
    unsigned ret = RTC_Init(&rtc_init);
    PWR_BackupAccessCmd(DISABLE);
}

void init_encoder_timer()
{
/* Initialize the corresponding input pins */
        GPIO_InitTypeDef GPIO_InitStructure;
/* Enable clock to GPIOD */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);

/* Initialize the timer */
/* Enable clock to TIM4 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

        TIM_TimeBaseInitTypeDef TIM_InitStructure;
        TIM_TimeBaseStructInit(&TIM_InitStructure);
        TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_InitStructure.TIM_Period = 0xFFFF;
        TIM_InitStructure.TIM_Prescaler = 0;
        TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM4, &TIM_InitStructure);

/* Set the timer up for quadrature decoding */
        TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
        TIM4->CNT = ENCODER_COUNTER_ZERO;
        TIM_Cmd(TIM4, ENABLE);
}

void Button_Init(void) {
    // use interrupts
    // STM_EVAL_PBInit(BUTTON_SW1, BUTTON_MODE_EXTI);
    // STM_EVAL_PBInit(BUTTON_SW2, BUTTON_MODE_EXTI);

    // dont use interrupts
    STM_EVAL_PBInit(BUTTON_SW1, BUTTON_MODE_GPIO);
    STM_EVAL_PBInit(BUTTON_SW2, BUTTON_MODE_GPIO);
}

/**
 * Delay function
 * @param nCount = Number of cycles to delay.
 */
void delay(vu32 nCount) {
    for (; nCount != 0; nCount--);
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

void init_internal_temperature(void) {

    
    /* ADC configuration */
    ADC_InitTypeDef ADC_InitStructure;
//    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    
    /* ADC Peripheral clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    ADC_DeInit();
    
//    ADC_CommonStructInit(&ADC_CommonInitStructure);
//    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
//    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
//    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
//    ADC_CommonInit(&ADC_CommonInitStructure);

    /* Configure the ADC1 */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_TempSensorVrefintCmd(ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);
    ADC_SoftwareStartConv(ADC1);
    
    uint16_t adc_val = ADC_GetConversionValue(ADC1);

    float adc_float = ((float)adc_val);
    float temp = (((float)adc_float-760.0f)/2.5f)+25.0f;
}