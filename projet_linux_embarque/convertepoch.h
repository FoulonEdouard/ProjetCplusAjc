#ifndef CONVERTEPOCH_H
#define CONVERTEPOCH_H
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>

class HistoryCache
{
public:
    HistoryCache();
    static std::string getTimeStamp(time_t epochTime, const char* format = "%Y-%m-%d")
    {
       char timestamp[64] = {0};
       strftime(timestamp, sizeof(timestamp), format, localtime(&epochTime));
       return timestamp;
    }
    static time_t convertTimeToEpoch(const char* theTime, const char* format = "%Y-%m-%d")
    {
       std::tm tmTime;
       memset(&tmTime, 0, sizeof(tmTime));
       strptime(theTime, format, &tmTime);
       return mktime(&tmTime);
    }
};

#endif // CONVERTEPOCH_H
