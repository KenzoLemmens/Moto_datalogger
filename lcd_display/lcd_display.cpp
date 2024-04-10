#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/time.h"
#include "lcd_display.hpp"
#include <sstream>

// Pin positions in LCDpins array
#define RS 4
#define E 5
// Pin values
#define HIGH 1
#define LOW 0
// LCD pin RS meaning
#define COMMAND 0
#define DATA 1
	
using namespace std;

bool cleared = true;
bool toInit = true;

uint32_t LCDdisplay::pin_values_to_mask(uint raw_bits[],int length) {   // Array of Bit 7, Bit 6, Bit 5, Bit 4, RS(, clock)
	uint32_t result = 0 ;
	uint pinArray[32] ;
	for (int i = 0 ; i < 32; i++) {pinArray[i] = 0;}
	for (int i = 0 ; i < length ; i++) {pinArray[this->LCDpins[i]]= raw_bits[i];}
	for (int i = 0 ; i < 32; i++) {
		result = result << 1 ;
		result += pinArray[31-i] ;
	}
	return result ;
};
	
	void LCDdisplay::uint_into_8bits(uint raw_bits[], uint one_byte) {  	
		for (int i = 0 ; i < 8 ; i++ ) {
			raw_bits[7-i] = one_byte % 2 ;
			one_byte = one_byte >> 1 ;
		}
	};
	
	void LCDdisplay::send_raw_data_one_cycle(uint raw_bits[]) { // Array of Bit 7, Bit 6, Bit 5, Bit 4, RS
		uint32_t bit_value = pin_values_to_mask(raw_bits,5) ;
		gpio_put_masked(this->LCDmask, bit_value) ;
		gpio_put(this->LCDpins[E], HIGH);
		sleep_us(50);
		gpio_put(this->LCDpins[E], LOW);
	};
		
	void LCDdisplay::send_full_byte(uint rs, uint databits[]) {
		uint rawbits[5];
		rawbits[4] = rs ;
		for (int i = 0 ; i<4 ; i++) { rawbits[i]=databits[i];}
		send_raw_data_one_cycle(rawbits);
		for (int i = 0; i<4 ; i++) { rawbits[i]=databits[i+4];}
		send_raw_data_one_cycle(rawbits);
	};
	
	
	LCDdisplay::LCDdisplay(int bit4_pin, int bit5_pin, int bit6_pin, int bit7_pin, int rs_pin, int e_pin, int width, int depth, MDataLogger* mDataLog) { // constructor
		this->LCDpins[0] = bit7_pin;
		this->LCDpins[1] = bit6_pin;
		this->LCDpins[2] = bit5_pin;
		this->LCDpins[3] = bit4_pin;
		this->LCDpins[4] = rs_pin;
		this->LCDpins[5] = e_pin;
		this->no_chars = width;
		this->no_lines = depth;
		this->logger = mDataLog;
		init();
	};

	void LCDdisplay::clear() {
		uint clear_display[8] = {0,0,0,0,0,0,0,1};
		send_full_byte(COMMAND, clear_display);
		sleep_us(10) ; // extra sleep due to equipment time needed to clear the display
	};

	void LCDdisplay::init() { // initialize the LCD
		sleep_ms(15);
		uint all_ones[6] = 			{1,1,1,1,1,1};
		uint functionSet[8] = 		{0,0,1,1,0,0,0,0};
		uint interfaceLength[8] = 	{0,0,1,0,0,0,0,0};
		uint lcdProps[8] = 			{0,0,1,0,1,0,0,0};
		uint lockLcdProps[8] = 		{0,0,0,0,1,0,0,0};
		uint lcdClear[8] = 			{0,0,0,0,0,0,0,1};
		uint entryMode[8] = 		{0,0,0,0,0,1,1,0};
		uint cursorMode[8] = 		{0,0,0,0,1,1,0,0};
		
		//set mask, initialize masked pins and set to LOW 
		this->LCDmask_c = pin_values_to_mask(all_ones,6);
		this->LCDmask = pin_values_to_mask(all_ones,5);
		gpio_init_mask(this->LCDmask_c);   			// init all LCDpins
		gpio_set_dir_out_masked(this->LCDmask_c);	// Set as output all LCDpins
		gpio_clr_mask(this->LCDmask_c);				// LOW on all LCD pins 
		
		send_full_byte(COMMAND, functionSet);
		sleep_us(4100);
		send_full_byte(COMMAND, functionSet);
		sleep_us(100);
		send_full_byte(COMMAND, functionSet);

		send_full_byte(COMMAND, interfaceLength);
		send_full_byte(COMMAND, lcdProps);
		send_full_byte(COMMAND, entryMode);
		
		send_full_byte(COMMAND, lcdClear);
		sleep_ms(5);
		send_full_byte(COMMAND, cursorMode);

		sleep_ms(50);
		print("     ____  ___ ", 0, 0);
		print("    |__ / |_  )", 0, 1);
		print("     |_ |  / / ", 0, 2);
		print("    |___/ /___|", 0, 3);
	
	};

	void LCDdisplay::goto_pos(char pos_i, char line) {
		uint eight_bits[8];

		if (line == 0 || line == 1) pos_i = 64*line+ pos_i + 0b10000000; 
		else pos_i = 64*((line-1)/2) + 20 + pos_i + 0b10000000;

		uint_into_8bits(eight_bits,pos_i);
		send_full_byte(COMMAND,eight_bits);
	};
	
	void LCDdisplay::print(const char * str, char pos_i, char line) {
		goto_pos(pos_i, line);
		uint eight_bits[8];
		int i = 0 ;
		while (str[i] != 0) {
			uint_into_8bits(eight_bits,(uint)(str[i]));
			send_full_byte(DATA, eight_bits);
			++i;
		}
	};
				

void LCDdisplay::printLast(const char * str) { print(str, 0, 0); }

void LCDdisplay::printDiff(const char * str) { print(str, 13, 0); }

void LCDdisplay::printBest(const char * str) { print(str, 0, 1); }

void LCDdisplay::printSession() {
	if (logger->recording) {
		if(toInit) {
			clear();
			toInit = !toInit;
		}
		string session = logger->getSessionTimeStr();
		print(session.c_str(), 0, 3);
		cleared = false;
	} else if (!cleared) {
		logger->resetLogger();
		print("--:--", 0, 3);
		cleared = !cleared;
	}
}