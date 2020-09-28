#ifndef weather__h
#define weather__h

#include <stdlib.h>
typedef struct Weather Weather;

int performCurlRequest(void *zp, void *ll,  void *dest);
size_t write_data(void *data, size_t size, size_t nmemb, void *userp);
void obtainWeatherData();
void extractZipJSON();

#endif
