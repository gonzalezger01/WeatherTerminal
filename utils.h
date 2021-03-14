
#ifndef utils__h
#define utils__h

#include <stddef.h>
#include "weather.h"
#include <bits/types/FILE.h>

struct Weather;

int createlatLonConfigFile(const struct Weather* weather);
double collectCoordinates(FILE* fp, struct Weather* weather);
void checkAndFree(void *pointer);
void extractValue(char* string);
size_t length(char const* data);
char* mallocAndCopyStr(char *string);
size_t trimString(char* string);
void trimNewline(char* line);
void printWeatherStructContents(const struct Weather* weather);

#endif
