#include "pico/stdlib.h"
#include <stdio.h>
#include "lcd_display.hpp"
#include "moto_datalogger.hpp"
#include <sstream>
#include "hardware/gpio.h"

#define REC_PIN 6
#define debounce 250

using namespace std;

MDataLogger logger;

LCDdisplay myLCD(2,3,4,5,14,15,20,4, &logger); // DB4, DB5, DB6, DB7, RS, E, character_width, no_of_lines

void recordInterrupt(uint gpio, uint32_t events) {
	uint triggerd = time_us_64() / 1000;
	if ((triggerd - logger.lastTrigger) > debounce) {
		logger.recording  = !logger.recording;
		logger.startTime = triggerd;
		logger.lastTrigger = triggerd;
	}
}

int main() {
	myLCD.init();
	stdio_init_all();
    gpio_set_irq_enabled_with_callback(REC_PIN, GPIO_IRQ_EDGE_RISE, true, &recordInterrupt);
	
	myLCD.printLast("L -:--.---");
	myLCD.printDiff("-xx.xxx");
	myLCD.printBest("B -:--.---");


	while(true) {

		if (logger.recording){
			myLCD.printLast("L 1:00.000");
			myLCD.printDiff("-00.500");
			myLCD.printBest("B 1:00.000");
		} 

		myLCD.printSession();
	}
	return 0;
};