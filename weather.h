#ifndef weather__h
#define weather__h

#include <stdlib.h>
#include <stdbool.h>

struct Day {
  char *name;
  char *startTime;
  char *endTime;
  bool isDayTime;
  int temperature;
  char *windSpeed;
  char * windDirection;
  char *shortForecast;
  char *detailedForecast;
  double rainPercentage;
};

struct Weather {
  char *city;
  char *state;
  char *currentShortForecast;
  time_t lastUpdateTime;
  struct Day weatherForecast[14];
  char *zipCode;
  char lat;
  char lon;
  char *gridId;
};

void obtainLatLongData(void *zip, struct Weather **dataDest);
void obtainWeatherData();

#endif
