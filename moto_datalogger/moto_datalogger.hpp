#pragma once

#include <sstream>

using namespace std;

class MDataLogger {
	private:

	public:
    MDataLogger();
    uint startTime;
    bool recording;
    uint bestLap; // 
    uint lastLap;
    uint startLap; 
    int lapDiff;
    string lastStr;
    string bestStr;
    string diffStr;

    string intToStr(uint number);
    string getSessionTimeStr();
    uint getSessionTime();
    void recordLap(long callTime);
    string msToStr(char type, int ms);
    void resetLogger();
};