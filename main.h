//
// Title:	        Pico-mposite Video Output
// Author:	        Dean Belfield
// Created:	        26/01/2021
// Last Updated:	01/03/2022
//
// Modinfo:
// 20/02/2022:      Added demo_terminal
// 01/03/2022:      Added colour to the demos

#pragma once

#include "config.h"

#if opt_colour == 0
    #define col_black   0x00
    #define col_grey    0x0C
    #define col_white   0x0F
	// Colours are defined by their monochrome brightness according to 
	// Y = 0.21 R + 0.71 G + 0.072 B
	#define col_red     3
    #define col_green   11
    #define col_blue    1
    #define col_yellow  (col_red+col_green)
    #define col_magenta (col_red+col_blue)
    #define col_cyan    (col_blue+col_green)
#elif opt_colour == 1
    #define col_black   rgb(0,0,0)
    #define col_grey    rgb(6,6,6)
    #define col_white   rgb(7,7,7)
    #define col_red     rgb(7,0,0)
    #define col_green   rgb(0,7,0)
    #define col_blue    rgb(0,0,7)
    #define col_yellow  rgb(7,7,0)
    #define col_magenta rgb(7,0,7)
    #define col_cyan    rgb(0,7,7)
#else
	#define col_black   0x0
    #define col_white   0x7
    #define col_red     0x1
    #define col_green   0x2
    #define col_blue    0x4
    #define col_yellow  0x3
    #define col_magenta 0x5
    #define col_cyan    0x6

	
#endif

const uint BUTTON_PIN = 23;
// Set to true for most pins, or false if using the BOOT button
// on a Pimoroni Pico-Lipo
const bool BUTTON_PIN_PULLUP = false;
const uint LED_PIN = 25;

int main(void);

void draw_pattern(int i,int sub_c);
void cross(int sub_c);
void dots(int sub_c);
void grid(bool draw_h, bool draw_v, int sub_c);
void full_colour(int sub_c);
void colour_bars_v(int sub_c);
void colour_bars_h(int sub_c);
void chess(int sub_c);
void test_circle(void);
