#include "pico/stdlib.h"
#include <stdio.h>
#include "lcd_display.hpp"
#include "moto_datalogger.hpp"
#include <sstream>
#include "hardware/gpio.h"

#define REC_PIN 6
#define debounce 250

using namespace std;
uint lastTriggerInt;
uint lastTriggerRecord;

MDataLogger logger;

LCDdisplay myLCD(2,3,4,5,14,15,20,4, &logger); // DB4, DB5, DB6, DB7, RS, E, character_width, no_of_lines

void recordInterrupt(uint gpio, uint32_t events) {
	uint triggerd = time_us_64() / 1000;
	if ((triggerd - lastTriggerInt) > debounce) {
		logger.recording  = !logger.recording;
		logger.startTime = triggerd;
		lastTriggerInt = triggerd;
	}
}

int main() {
	long now = 0;
	gpio_init(7);
    gpio_set_irq_enabled_with_callback(REC_PIN, GPIO_IRQ_EDGE_RISE, true, &recordInterrupt);

	while(true) {
		if (gpio_get(7)) {
			now = time_us_64() / 1000;
			if (((now) - lastTriggerRecord) > debounce && logger.recording) {
				logger.recordLap(now);
				lastTriggerRecord = now;
			}
		}

		if (logger.recording){
			myLCD.printLast(logger.lastStr.c_str());
			myLCD.printDiff(logger.diffStr.c_str());
			myLCD.printBest(logger.bestStr.c_str());
		}
		myLCD.printSession();
	}
	return 0;
};