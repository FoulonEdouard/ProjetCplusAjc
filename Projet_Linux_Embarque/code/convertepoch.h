#ifndef CONVERTEPOCH_H
#define CONVERTEPOCH_H
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>

/**
 * @class HistoryCache
 * Il s'agit de la classe qui va nous permettre de passer une date au format epoch ou inversement
 */


class HistoryCache
{
public:
    /**
     * @fn HistoryCache()
     * @brief Il s'agit du constructeur de Loader
     */
    HistoryCache();
    /**
     * @fn static std::string getTimeStamp(time_t epochTime, const char* format = "%Y-%m-%d")
     * La fonction qui va renvoyé une date epoch au format string
     */

    static std::string getTimeStamp(time_t epochTime, const char* format = "%Y-%m-%d")
    {
       char timestamp[64] = {0};
       strftime(timestamp, sizeof(timestamp), format, localtime(&epochTime));
       return timestamp;
    }

    /**
     * @fn static time_t convertTimeToEpoch(const char* theTime, const char* format = "%Y-%m-%d")
     * La fonction qui va renvoyé une date au format epoch
     */
    static time_t convertTimeToEpoch(const char* theTime, const char* format = "%Y-%m-%d")
    {
       std::tm tmTime;
       memset(&tmTime, 0, sizeof(tmTime));
       strptime(theTime, format, &tmTime);
       return mktime(&tmTime);
    }
};

#endif // CONVERTEPOCH_H
