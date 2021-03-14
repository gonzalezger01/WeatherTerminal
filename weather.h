#ifndef weather__h
#define weather__h

#include "cJSON.h"
#include "constants.h"
#include "utils.h"
#include <time.h>
#include <stdbool.h>

struct Day {
    char* name;
    char* startTime;
    char* endTime;
    bool isDayTime;
    int temperature;
    char* windSpeed;
    char* windDirection;
    char* shortForecast;
    char* detailedForecast;
};

struct Weather {
    char* city;
    char* state;
    char* zipCode;
    double lat;
    double lon;
    char* gridId;
    int gridX;
    int gridY;
    char* lastUpdateTime;
    char* currentShortForecast;
    struct Day weatherForecast[14];
};

void obtainLatLongData(void* zip, struct Weather* weatherDest);
void obtainWeatherOfficeData(struct Weather* weatherStruct);
void obtainWeatherData(struct Weather* weatherStruct);

void initalizeWeatherStruct(struct Weather* weatherStruct);
void freeWeatherStruct(struct Weather* weatherStruct);

#endif
