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

	const int led_delay = 30;
	const int long_count = 150;
	
    int i =0;
	int sub_c=0;
	int but = 1;
	int but_ready_for_down = true;
	int but_ready_for_up = false;
	int but_up_count = 0;
	int but_down_count = 0;
	int led_count = 0;
    colour_bars_h(0);
	

	while(true) {

		sleep_ms(10);
		led_count ++;
		if (led_count<led_delay*(i+1))
			gpio_put(LED_PIN,(led_count%led_delay)<5);
		else if (led_count <led_delay*(i+1)+60)
			gpio_put(LED_PIN,0);
		else
			led_count = 0;
			
		but = gpio_get(BUTTON_PIN);
		if (!but) {
			if (but_ready_for_down) {
				but_ready_for_down = false;
				but_ready_for_up = true;
			}	
			but_down_count++;
			if (but_down_count>long_count) {
				sub_c ++;
				but_down_count=0;
				but_ready_for_up = false;
				draw_pattern(i, sub_c);
			}
		} else {
			if (but_ready_for_up) {
				sub_c=0;
				but_up_count = 0;
				i=(i+1)%10;
				draw_pattern(i, sub_c);
			}
			if (but_up_count < 2) 
				but_up_count ++;
			else
				but_ready_for_down = true;
			but_ready_for_up = false;
			but_down_count = 0;
		}
	}
}

void draw_pattern(int i, int sub_c) {
	switch (i) {
		case 0: colour_bars_h(sub_c);
		break;
		case 1: colour_bars_v(sub_c);
		break;
		case 2: full_colour(sub_c);
		break;
		case 3: grid(true,false,sub_c);
		break;
		case 4: grid(false,true,sub_c);
		break;
		case 5: grid(true,true,sub_c);
		break;
		case 6: dots(sub_c);
		break;
		case 7: cross(sub_c);
		break;
		case 8: chess(sub_c);
		break;
		default:
			test_circle();
	}
}

unsigned char sub_c_2_col(int sub_c) {
	unsigned char c[4] = {col_white, col_red, col_green, col_blue};
	return c[sub_c%4];
}

void grid(bool draw_h, bool draw_v,int sub_c) {
	set_border(col_black);
	cls(col_black);
	unsigned char c = sub_c_2_col(sub_c);
	const int d=(width+10)/12;
	if (draw_v)
		for (int x = 0; x<8; x++) {
			draw_line(width/2+d*x,0,width/2+d*x,height-1,c);
			if (x>0)
				draw_line(width/2-d*x,0,width/2-d*x,height-1,c);
		}
	if (draw_h)
		for (int y = 0; y<6; y++) {
			draw_line(0,height/2+d*y,width-1,height/2+d*y,c);
			if (y>0)
				draw_line(0,height/2-d*y,width-1,height/2-d*y,c);
		}
	draw_circle(width/2,height/2,d/2,c,0);
}

void dots(int sub_c) {
	set_border(col_black);
	cls(col_black);
	const int d=(width+10)/12;
	for (int x = -7; x<8; x++)
		for (int y = -5; y<6; y++)
			plot(width/2+x*d,height/2+y*d,sub_c_2_col(sub_c));
}

void cross(int sub_c) {
	set_border(col_black);
	cls(col_black);
	draw_line(0,height/2,width-1,height/2,sub_c_2_col(sub_c));
	draw_line(width/2,0,width/2,height-1,sub_c_2_col(sub_c));
}

void full_colour(int sub_c) {
	set_border(sub_c_2_col(sub_c));
	cls(sub_c_2_col(sub_c));
}

void colour_bars_v(int sub_c) {
	const int rectw = width/8;
	const unsigned char c[8] = {col_white, col_yellow, col_cyan, col_green, col_magenta, col_red, col_blue, col_black};
	set_border(col_black);
	cls(col_black);
	for (int i =0; i<8 ; i++) {
		draw_rect(i*rectw,0,i*rectw+rectw-1,height,c[(sub_c%2)?7-i:i],1);
	}
}

void colour_bars_h(int sub_c) {
	const int recth = height/8;
	const unsigned char c[8] = {col_white, col_yellow, col_cyan, col_green, col_magenta, col_red, col_blue, col_black};
	set_border(col_black);
	cls(col_black);
	for (int i =0; i<8 ; i++) {
		draw_rect(0,i*recth,width, i*recth+recth-1,c[(sub_c%2)?i:7-i],1);
	}
	print_string(0,recth/2,"github.com/sharpie7/pico-pattern",col_black,col_white);
}

void chess(int sub_c) {
	set_border(col_black);
	cls(col_black);
	const int n=5;
	const int d=(width+10)/(2*n);
	for (int x = -n; x<n; x++) 
		for (int y = -n; y<n; y++)
			if ((x+y)%2)
				draw_rect(width/2+x*d,height/2+y*d,width/2+x*d+d-1,height/2+y*d+d-1,sub_c_2_col(sub_c),1);
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

