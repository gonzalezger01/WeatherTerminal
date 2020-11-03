#ifndef weather__h
#define weather__h

#include <stdlib.h>
struct Location {
  char *lat;
  char *lon;
};


struct Weather{
  char *city;
  char *state;
  char *weatherStatus;
  int temperature;
  double rainPercentage;
  time_t lastUpdateTime;
  char *zipCode;
  double lattitude;
  double longitude;
};

void obtainLatLongData(void *zip, void *dataDest);
void obtainWeatherData();

#endif
