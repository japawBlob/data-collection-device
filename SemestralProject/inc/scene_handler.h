
#ifndef __SCENE_HANDLER_H
#define __SCENE_HANDLER_H

#include "lcd_framebuffer.h"
#include "lcd_st7565p.h"
#include "logger.h"

uint8_t frame_buffer[128*64];

#define HOME_HUB 0
#define SET_THERMOSTAT 1
#define DATA_SCREEN 2

extern uint8_t current_scene;
extern uint16_t current_line;
extern uint8_t desired_temp;


void init_scenes();

void set_scene(uint8_t scene, struct data_entry de);
void set_home_hub();
void set_thermostat();
void set_data();
void update_scene();
void sent_temp();




#endif