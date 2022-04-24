//
// Title:	        Pico-mposite Video Output
// Description:		A hacked-together composite video output for the Raspberry Pi Pico
// Author:	        Dean Belfield
// Created:	        02/02/2021
// Last Updated:	01/03/2022
// 
// Modinfo:
// 04/02/2022:      Demos now set the border colour
// 05/02/2022:      Added support for colour
// 20/02/2022:      Added demo_terminal
// 01/03/2022:      Added colour to the demos

#include <stdlib.h>
#include <math.h>

#include "memory.h"
#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"  
#include "hardware/gpio.h" 

#include "graphics.h"
#include "cvideo.h"

#include "main.h"



// The main loop
//
int main() {
    initialise_cvideo();    // Initialise the composite video stuff
	gpio_init(BUTTON_PIN);
	if (BUTTON_PIN_PULLUP)
		gpio_pull_up(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);


    int i =0;
	int but = 1;
	int but_ready_for_down = true;
	int but_ready_for_up = false;
	int but_up_count = 0;
	int led_count = 0;
    colour_bars();
	

	while(true) {
		sleep_ms(10);
		led_count ++;
		if (led_count<40*(i+1))
			gpio_put(LED_PIN,(led_count%40)<5);
		else if (led_count <40*(i+1)+60)
			gpio_put(LED_PIN,0);
		else
			led_count = 0;
			
		but = gpio_get(BUTTON_PIN);
		if (!but) {
				if (but_ready_for_down) {
					i=(i+1)%6;
					switch (i) {
					case 0: colour_bars();
					break;
					case 1: full_white();
					break;
					case 2: grid();
					break;
					case 3: dots();
					break;
					case 4: cross();
					break;
					default:
						test_circle();
					}
				}
				but_ready_for_down = false;
				but_ready_for_up = true;
		} else {
			if (but_ready_for_up) {
				but_up_count = 0;
			}
			if (but_up_count < 2) 
				but_up_count ++;
			else
				but_ready_for_down = true;
			but_ready_for_up = false;
		}
	}
}

void grid() {
	set_border(col_black);
	cls(col_black);
	const int d=(width+10)/6;
	for (int x = 0; x<3; x++) {
		draw_line(width/2+d*x,0,width/2+d*x,height-1,col_white);
		if (x>0)
			draw_line(width/2-d*x,0,width/2-d*x,height-1,col_white);
	}
	for (int y = 0; y<3; y++) {
		draw_line(0,height/2+d*y,width-1,height/2+d*y,col_white);
		if (y>0)
			draw_line(0,height/2-d*y,width-1,height/2-d*y,col_white);
	}
}

void dots() {
	set_border(col_black);
	cls(col_black);
	const int d=(width+10)/12;
	for (int x = -7; x<8; x++)
		for (int y = -5; y<6; y++)
			plot(width/2+x*d,height/2+y*d,col_white);
}

void cross() {
	set_border(col_black);
	cls(col_black);
	draw_line(0,height/2,width-1,height/2,col_white);
	draw_line(width/2,0,width/2,height-1,col_white);
}

void full_white() {
	set_border(col_white);
	cls(col_white);
}

void colour_bars() {
	const int rectw = width/8;
	const unsigned char c[8] = {col_white, col_yellow, col_cyan, col_green, col_magenta, col_red, col_blue, col_black};
	set_border(col_black);
	cls(col_black);
	for (int i =0; i<8 ; i++) {
		draw_rect(i*rectw,0,i*rectw+rectw-1,height,c[i],1);
	}
}
void test_circle() {
    set_border(col_black);
	cls(col_green);

	draw_line(0,0,width-1,0,col_white);
	draw_line(width-1,0,width-1,height-1,col_white);
	draw_line(width-1,height-1,0,height-1,col_white);
	draw_line(0,height-1,0,0,col_white);
	draw_circle(width/2,height/2,height/2-10,col_black,0);
	

}

