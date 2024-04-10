#include "pico/stdlib.h"
#include "pico/time.h"
#include "moto_datalogger.hpp"
#include <sstream>
#include "hardware/gpio.h"
#include <array>

using namespace std;

MDataLogger::MDataLogger() {
	this->startTime = 0;
	this->recording = false;
	this->lastLap = 0;
	this->startLap = 0;
	this->lastStr = "L -:--.---";
	this->bestStr = "B -:--.---";
};

string MDataLogger::intToStr(uint number) {
	string ossVal;
	ostringstream oss;
	oss << number;
	ossVal = oss.str();
	ossVal = (ossVal.length() < 2 ) ? '0' + ossVal : ossVal;
	return ossVal;
} 

uint MDataLogger::getSessionTime() { return (time_us_64() / 1000) - startTime; }
string MDataLogger::getSessionTimeStr() { return msToStr('S', getSessionTime()); }

void MDataLogger::recordLap(long callTime) {
	if(startLap != 0){
		uint lap = callTime - startLap;
		lapDiff = (lastLap != 0 ) ? lap - lastLap : 0;
		lastLap = lap;
		bestLap = (lastLap < bestLap || bestLap == 0) ? lastLap : bestLap;
		lastStr = "L" + msToStr('L', lastLap);
		lastStr[1] = ' ';
		bestStr = "B" + msToStr('L', bestLap);
		bestStr[1] = ' ';
		diffStr = msToStr('D', lapDiff);
	} else diffStr = "~00.000";

	startLap = callTime;
}

string MDataLogger::msToStr(char type, int ms){
	char sign = (ms < 0) ? '-' : '+';
	ms = abs(ms);
	uint seconds = ms / 1000;
	uint minutes = seconds / 60;
	uint hours = minutes / 60;
	ms = ms % 1000;
	seconds = seconds % 60;
	minutes = minutes % 60;
	hours = hours % 24;

	string mS = intToStr(ms);
	string s = intToStr(seconds); // Maybe convert to char array passing in one call. 
	string m = intToStr(minutes);
	string h = intToStr(hours);

	string retVal = "";

	switch (type){
		case 'L':
			retVal = m + ":" + s + "." + mS;
			break;
		case 'D':
			retVal = sign + s + "." + mS;
			break;
		case 'S':
			retVal = m + ":" + s;
			break; 
		default: ;
	}
	return retVal;
}

void MDataLogger::resetLogger(){
	diffStr = " --.---";
	lastStr = "L -:--.---";
	bestStr = "B -:--.---";
	lapDiff = 0;
	lastLap = 0;
	bestLap = 0;
	startLap = 0;
}