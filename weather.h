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
void extractZipJSON();

size_t length(void *data);
size_t write_data(void *data, size_t size, size_t nmemb, void *userp);
int performCurlRequest(void *api,  void *dest);

#endif
