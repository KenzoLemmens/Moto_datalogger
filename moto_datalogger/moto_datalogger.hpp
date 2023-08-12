#pragma once

#include <sstream>

using namespace std;

class MDataLogger {
	private:

	public:
    MDataLogger();
    uint startTime;
    bool recording;
    uint lastTrigger;

    string intToStr(short number);
    string getSessionTimeStr();
    uint getSessionTime();
    
				
};