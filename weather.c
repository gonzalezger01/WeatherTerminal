#include "weather.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    LAT_LONG_API_ENUM = 0,
    WEATHER_API_FORECAST_ENUM = 1,
    WEATHER_API_OFFICE_ENUM = 2
} API_CALLS;

struct Memory {
    char* response;
    size_t size;
};

static int performCurlRequest(void* api, struct Weather* weatherP, API_CALLS apiCallType);
static size_t write_data(void* data, size_t size, size_t nmemb, void* userp);
static void extractZipJSON(void* jsonP, struct Weather* weatherP);
static void extractOfficeXYJSON(void* jsonP, struct Weather* weatherP);
static void extractWeatherForecastJSON(void* jsonP, struct Weather* weatherP);

static char* checkAndCopycJSONStr(const cJSON* cJson);
int checkAndCopycJSONInt(const cJSON* cJson);
bool checkAndCopycJSONBool(const cJSON* cJson);

void obtainLatLongData(void* zip, struct Weather* weatherP) {
    char* zipCode = (char*)zip;
    char* API = calloc(sizeof(char), (length(LAT_LONG_API) + length(zipCode)));
    strcat(API, LAT_LONG_API);
    strcat(API, zipCode);
    performCurlRequest(API, weatherP, LAT_LONG_API_ENUM);
    free(API);
}

void obtainWeatherOfficeData(struct Weather* weather) {
    const int latLongLength = 20;
    char* API = calloc(sizeof(char), (length(WEATHER_API_OFFICE) + latLongLength));
    strcat(API, WEATHER_API_OFFICE);
    char* weatherString = (char*)malloc(sizeof(char) * 6);
    sprintf(weatherString, "%.4f", weather->lat);
    strcat(API, weatherString);
    strcat(API, ",");
    sprintf(weatherString, "%.4f", weather->lon);
    strcat(API, weatherString);
    performCurlRequest(API, weather, WEATHER_API_OFFICE_ENUM);
    free(API);
    free(weatherString);
}

void obtainWeatherData(struct Weather* weather) {
    const int latLongLength = 10;
    char* API = calloc(sizeof(char), (length(WEATHER_API_FORECAST) + latLongLength));
    char* gridVal = (char*)malloc(sizeof(char) * 4);
    strcat(API, WEATHER_API_FORECAST);
    strcat(API, weather->gridId);
    strcat(API, "/");
    sprintf(gridVal, "%d", weather->gridX);
    strcat(API, gridVal);
    strcat(API, ",");
    sprintf(gridVal, "%d", weather->gridY);
    strcat(API, gridVal);
    strcat(API, "/forecast");
    performCurlRequest(API, weather, WEATHER_API_FORECAST_ENUM);
    free(gridVal);
    free(API);
}

//most of the code to perform curl request comes from curl's website
static int  performCurlRequest(void* api, struct Weather* weatherP, API_CALLS apiCallType) {
    CURL* curl;
    CURLcode res;

    char* url;
    curl = curl_easy_init();
    if (curl != NULL && api != NULL) {
        url = (char*)api;
        curl_easy_setopt(curl, CURLOPT_URL, url);

        if (apiCallType == WEATHER_API_OFFICE_ENUM || apiCallType == WEATHER_API_FORECAST_ENUM)
            curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);

        struct Memory mem;
        mem.response = calloc(sizeof(char), 100);
        mem.size = 0;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&mem);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Error with curl: %d\n", res);
            return -1;
        }

        switch (apiCallType) {
        case LAT_LONG_API_ENUM:
            extractZipJSON(mem.response, weatherP);
            break;
        case WEATHER_API_OFFICE_ENUM:
            extractOfficeXYJSON(mem.response, weatherP);
            break;
        case WEATHER_API_FORECAST_ENUM:
            extractWeatherForecastJSON(mem.response, weatherP);
            break;
        }

        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return 0;
}

static size_t write_data(void* data, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct Memory* mem = (struct Memory*)userp;

    char* ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL)
        return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

static void extractZipJSON(void* jsonP, struct Weather* weatherP) {
    char* jsonString = (char*)jsonP;
    cJSON* json = cJSON_Parse(jsonString);
    const cJSON* records = NULL;
    cJSON* record = NULL;

    records = cJSON_GetObjectItemCaseSensitive(json, "records");
    cJSON_ArrayForEach(record, records) {
        const cJSON* fields = NULL;
        const cJSON* state = NULL;
        const cJSON* city = NULL;
        const cJSON* latitude = NULL;
        const cJSON* longitude = NULL;

        fields = cJSON_GetObjectItemCaseSensitive(record, "fields");

        city = cJSON_GetObjectItemCaseSensitive(fields, "city");
        state = cJSON_GetObjectItemCaseSensitive(fields, "state");
        latitude = cJSON_GetObjectItemCaseSensitive(fields, "latitude");
        longitude = cJSON_GetObjectItemCaseSensitive(fields, "longitude");

        weatherP->city = checkAndCopycJSONStr(city);
        weatherP->state = checkAndCopycJSONStr(state);
        weatherP->lat = latitude->valuedouble;
        weatherP->lon = longitude->valuedouble;
    }
    cJSON_Delete(json);
}

void extractOfficeXYJSON(void* jsonP, struct Weather* weather) {
    char* jsonString = (char*)jsonP;
    cJSON* json = cJSON_Parse(jsonString);
    const cJSON* properties = cJSON_GetObjectItemCaseSensitive(json, "properties");
    const cJSON* gridId = NULL;
    const cJSON* gridX = NULL;
    const cJSON* gridY = NULL;

    if (properties == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    gridId = cJSON_GetObjectItemCaseSensitive(properties, "gridId");
    gridX = cJSON_GetObjectItemCaseSensitive(properties, "gridX");
    gridY = cJSON_GetObjectItemCaseSensitive(properties, "gridY");

    weather->gridId = checkAndCopycJSONStr(gridId);
    weather->gridX = gridX->valueint;
    weather->gridY = gridY->valueint;

    if (weather->city == NULL || weather->state == NULL) {
        const cJSON* relativeLocation = NULL;

        relativeLocation = cJSON_GetObjectItemCaseSensitive(properties, "relativeLocation");
        if (relativeLocation != NULL) {
            const cJSON* properties = NULL;
            const cJSON* city = NULL;
            const cJSON* state = NULL;

            properties = cJSON_GetObjectItemCaseSensitive(relativeLocation, "properties");
            if (properties != NULL) {
                city = cJSON_GetObjectItemCaseSensitive(properties, "city");
                state = cJSON_GetObjectItemCaseSensitive(properties, "state");
                weather->city = checkAndCopycJSONStr(city);
                weather->state = checkAndCopycJSONStr(state);
            }
        }
    }

    cJSON_Delete(json);
}

static void extractWeatherForecastJSON(void* jsonP, struct Weather* weatherP) {
    char* jsonString = (char*)jsonP;
    cJSON* json = cJSON_Parse(jsonString);
    const cJSON* properties = cJSON_GetObjectItemCaseSensitive(json, "properties");
    const cJSON* validTimes = NULL;
    const cJSON* periods = NULL;
    const cJSON* period = NULL;
    int counter = 0;

    validTimes = cJSON_GetObjectItemCaseSensitive(properties, "validTimes");
    weatherP->lastUpdateTime = checkAndCopycJSONStr(validTimes);

    periods = cJSON_GetObjectItemCaseSensitive(properties, "periods");
    if (periods != NULL) {
        const cJSON* name = NULL;
        const cJSON* startTime = NULL;
        const cJSON* endTime = NULL;
        const cJSON* isDaytime = NULL;
        const cJSON* temperature = NULL;
        const cJSON* windSpeed = NULL;
        const cJSON* windDirection = NULL;
        const cJSON* shortForecast = NULL;
        const cJSON* detailedForecast = NULL;

        cJSON_ArrayForEach(period, periods) {
            name = cJSON_GetObjectItemCaseSensitive(period, "name");
            startTime = cJSON_GetObjectItemCaseSensitive(period, "endTime");
            endTime = cJSON_GetObjectItemCaseSensitive(period, "startTime");
            isDaytime = cJSON_GetObjectItemCaseSensitive(period, "isDaytime");
            temperature = cJSON_GetObjectItemCaseSensitive(period, "temperature");
            windSpeed = cJSON_GetObjectItemCaseSensitive(period, "windSpeed");
            windDirection = cJSON_GetObjectItemCaseSensitive(period, "windDirection");
            shortForecast = cJSON_GetObjectItemCaseSensitive(period, "shortForecast");
            detailedForecast = cJSON_GetObjectItemCaseSensitive(period, "detailedForecast");

            weatherP->weatherForecast[counter].name = checkAndCopycJSONStr(name);
            weatherP->weatherForecast[counter].startTime = checkAndCopycJSONStr(startTime);
            weatherP->weatherForecast[counter].endTime = checkAndCopycJSONStr(endTime);
            weatherP->weatherForecast[counter].isDayTime = checkAndCopycJSONBool(isDaytime);
            weatherP->weatherForecast[counter].temperature = checkAndCopycJSONInt(temperature);
            weatherP->weatherForecast[counter].windSpeed = checkAndCopycJSONStr(windSpeed);
            weatherP->weatherForecast[counter].windDirection = checkAndCopycJSONStr(windDirection);
            weatherP->weatherForecast[counter].shortForecast = checkAndCopycJSONStr(shortForecast);
            weatherP->weatherForecast[counter].detailedForecast = checkAndCopycJSONStr(detailedForecast);
            counter++;
        }
    }

    cJSON_Delete(json);
}

char* checkAndCopycJSONStr(const cJSON* cJson) {
    if (cJSON_IsString(cJson) && (cJson->valuestring != NULL))
        return mallocAndCopyStr(cJson->valuestring);
    return NULL;
}

int checkAndCopycJSONInt(const cJSON* cJson) {
    if (cJSON_IsNumber(cJson))
        return cJson->valueint;
    return INVALID_TEMP;
}

bool checkAndCopycJSONBool(const cJSON* cJson) {
    if (cJSON_IsString(cJson) && (cJson->valuestring != NULL))
        return strcmp(cJson->valuestring, "true") == 0 ? true : false;
    return INVALID_TEMP;
}

void initalizeWeatherStruct(struct Weather* weatherStruct) {
    weatherStruct->city = NULL;
    weatherStruct->state = NULL;
    weatherStruct->zipCode = NULL;
    weatherStruct->gridId = NULL;
    weatherStruct->currentShortForecast = NULL;

    for (int i = 0; i < 14; i++) {
        struct Day day = weatherStruct->weatherForecast[i];
        day.name = NULL;
        day.startTime = NULL;
        day.endTime = NULL;
        day.windDirection = NULL;
        day.shortForecast = NULL;
        day.detailedForecast = NULL;
    }
}

void freeWeatherStruct(struct Weather* weatherStruct) {
    checkAndFree(weatherStruct->city);
    checkAndFree(weatherStruct->state);
    checkAndFree(weatherStruct->currentShortForecast);
    checkAndFree(weatherStruct->zipCode);
    checkAndFree(weatherStruct->gridId);

    if (weatherStruct->weatherForecast[0].name != NULL) {
        for (int i = 0; i < 14; i++) {
            struct Day day = weatherStruct->weatherForecast[i];
            checkAndFree(day.name);
            checkAndFree(day.startTime);
            checkAndFree(day.endTime);
            checkAndFree(day.windDirection);
            checkAndFree(day.shortForecast);
            checkAndFree(day.detailedForecast);
        }
    }
}