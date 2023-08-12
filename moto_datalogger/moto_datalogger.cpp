#include "pico/stdlib.h"
#include "pico/time.h"
#include "moto_datalogger.hpp"
#include <sstream>
#include "hardware/gpio.h"

using namespace std;

MDataLogger::MDataLogger() {
	this->startTime = 0;
	this->recording = false;
	this->lastTrigger = 0;
};

string MDataLogger::intToStr(short number) {
	string ossVal;
	ostringstream oss;
	oss << number;
	ossVal = oss.str();
	return ossVal;
}

uint MDataLogger::getSessionTime() {
	return (time_us_64() / 1000) - startTime;
}

string MDataLogger::getSessionTimeStr() {
	string retVal = "";
	if (recording) {
		short seconds = getSessionTime() / 1000;
		short minutes = seconds / 60;
		short hours = minutes / 60;
		seconds = seconds % 60;
		minutes = minutes % 60;
		hours = hours % 24;

		string secStr = intToStr(seconds);
		string minStr = intToStr(minutes);
		string hrStr = intToStr(hours);

		secStr = (secStr.length() < 2 ) ? '0' + secStr : secStr;
		minStr = (minStr.length() < 2 ) ? '0' + minStr : minStr;
		hrStr = (hrStr.length() < 2 ) ? '0' + hrStr : hrStr;

		retVal = hrStr + ':' + minStr + ':' + secStr;
	} 
	return retVal;
}

