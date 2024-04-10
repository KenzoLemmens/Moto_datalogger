#pragma once
#include "moto_datalogger.hpp"

class LCDdisplay {
	
	private:
	int LCDpins[6];
	uint32_t LCDmask_c = 0 ; // with clock
	uint32_t LCDmask = 0 ; //without clock
	int no_chars;
	int no_lines;
	int cursor_status[2] = {0,0};

	MDataLogger* logger;
	
	uint32_t pin_values_to_mask(uint raw_bits[],int length);

	
	void uint_into_8bits(uint raw_bits[], uint one_byte);
	
	void send_raw_data_one_cycle(uint raw_bits[]);
		
	void send_full_byte(uint rs, uint databits[]);
	
	
	public:
	
	LCDdisplay(int bit4_pin, int bit5_pin, int bit6_pin, int bit7_pin, int rs_pin, int e_pin, int width, int depth, MDataLogger* logger);
	
	void clear();

	void cursor_on(bool blink);
	
	void init() ;

	void goto_pos(char pos_i, char line);
	
	void print(const char * str, char pos_i, char line);
	
	void display_off() ;
	
	void display_on() ;

	void printLast(const char * str);

	void printBest(const char * str);

	void printDiff(const char * str);

	void printSession();
				
};

