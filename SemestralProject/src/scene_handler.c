#include "scene_handler.h"
#include "logger.h"

uint8_t current_scene = 0;
uint16_t current_line = 0;
uint8_t desired_temp = 25;

void set_home_hub(){
    LCD_Clear();
    LCD_DisplayStringLineEx(Line0, "      HOME HUB   ", 0);
    char buff [100];
    sprintf(buff, "Temp: %d C", current_entry.temperature);
    LCD_DisplayStringLineEx(Line2, buff, 0);
    sprintf(buff, "Humidity: %d %%", current_entry.humidity);
    LCD_DisplayStringLineEx(Line3, buff, 0);
    char * seismic;
    if(current_entry.vibrations==SEISMIC_NONE){
        seismic = "NONE";
    } else if (current_entry.vibrations==SEISMIC_LOW){
        seismic = "LOW";
    } else if (current_entry.vibrations==SEISMIC_MEDIUM){
        seismic = "MID";
    } else if (current_entry.vibrations==SEISMIC_HIGH){
        seismic = "HIGH";
    }
    sprintf(buff, "Seismic: %s", seismic);
    LCD_DisplayStringLineEx(Line4, buff, 0);
    if(get_number_of_records() >= RECORDS_MAX_NUM){
        LCD_DisplayStringLineEx(Line5, "MEMORY FULL!", 0);
    } else {
        sprintf(buff, "Free space: %d", RECORDS_MAX_NUM-get_number_of_records());
        LCD_DisplayStringLineEx(Line6, buff, 0);
    }
    LCD_DisplayStringLineEx(Line7, "Thermostat | Data", 0);
    current_scene = HOME_HUB;
}

void set_thermostat(){
    LCD_Clear();
    LCD_DisplayStringLineEx(Line0, "   Thermostat   ", 0);
    char buff [100];
    sprintf(buff, "Desired temp: %d C", desired_temp);
    LCD_DisplayStringLineEx(Line3, buff, 0);

    LCD_DisplayStringLineEx(Line7, "Confirm | Back", 0);
    current_scene = SET_THERMOSTAT;
}

void set_data(){
    struct data_entry * entries = get_entries();
    uint16_t size = get_number_of_records()-current_line;
    LCD_Clear();
    LCD_DisplayStringLineEx(Line0, "      Data   ", 0);
    LCD_DisplayStringLineEx(Line1, "Time|Temp|Humid|Seism", 0);
    int i;
    unsigned len = size > 5 ? 5 : size;
    for (i = 0; i<len; i++){
        uint8_t in = i+current_line;
        uint8_t buf [100];
        char * seismic;
        if(entries[in].vibrations==SEISMIC_NONE){
            seismic = "NONE";
        } else if (entries[in].vibrations==SEISMIC_LOW){
            seismic = "LOW";
        } else if (entries[in].vibrations==SEISMIC_MEDIUM){
            seismic = "MID";
        } else if (entries[in].vibrations==SEISMIC_HIGH){
            seismic = "HIGH";
        }
        sprintf(buf, "%4d %4d %5d %5s", entries[in].time_stamp, entries[in].temperature, entries[in].humidity, seismic);
        LCD_DisplayStringLineEx(i+2, buf, 0);
    }
//    char buff [100];
//    sprintf(buff, "Desired temp: %d C", desired_temp);
//    LCD_DisplayStringLineEx(Line3, buff, 0);

    LCD_DisplayStringLineEx(Line7, "Send data | Back", 0);
    current_scene = DATA_SCREEN;
}

void set_scene(uint8_t scene, struct data_entry de){
    if (scene == HOME_HUB){
        // FB_Display(home_scene);
        set_home_hub(de);
    } else if (scene == SET_THERMOSTAT){
        set_thermostat(25);
    } else if (scene == DATA_SCREEN){
        
    }
}

void update_scene(){
    if(current_scene == HOME_HUB){
        set_home_hub();
    } else if (current_scene == SET_THERMOSTAT){
        set_thermostat();
    } else if (current_scene == DATA_SCREEN){
        set_data();
    }
}

void sent_temp(){
    iprintf("Desired temperature: %d\n\r", desired_temp);
    USART_SendData(USART3, 0x03);
}
