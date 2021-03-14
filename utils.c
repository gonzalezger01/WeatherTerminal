#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <bits/types/FILE.h>

int createlatLonConfigFile(const struct Weather* weather) {
    printf("Creating file..\n");
    FILE* configFile = fopen(CONFIG_FILE, "w");

    if (configFile != NULL) {
        fprintf(configFile, "lat=%.5f\n", weather->lat);
        fprintf(configFile, "lon=%.5f\n", weather->lon);
        fclose(configFile);
        return 1;
    }
    return -1;
}

double collectCoordinates(FILE* fp, struct Weather* weather) {
    int readLen = 20;
    char* str = malloc(sizeof(char) * readLen);
    char* end;
    double coordinate = 0.0;

    if (fgets(str, readLen, fp) != NULL) {
        extractValue(str);
        weather->lat = strtod(str, &end);
    }


    if (fgets(str, readLen, fp) != NULL) {
        extractValue(str);
        weather->lon = strtod(str, &end);
    }

    if (ferror(fp)) {
        puts("something broke");
        perror("collectCoordinates:");
    }
    free(str);
    fclose(fp);
    return coordinate;
}

void extractValue(char* string) {
    int counter = 0;

    while (*(string + counter) != '=') {
        counter++;
    }
    counter++;

    char* temporaryStorage = calloc(sizeof(char), 50);
    if (temporaryStorage != NULL) {
        int index = 0;
        while (*(string + counter + index) != '\0' && *(string + counter + index) != '\n') {
            *(temporaryStorage + index) = *(string + counter + index);
            index++;
        }

        memcpy(string, temporaryStorage, sizeof(char) * index);
        free(temporaryStorage);
    }
}

size_t length(char const* data) {
    size_t counter = 0;
    while (*(data + counter) != '\0')
        counter++;
    return counter + 1;
}

void trimNewline(char* line) {
    size_t index = 0;
    while (*(line + index) != '\0') {
        if (*(line + index) == '\n') {
            break;
        }
        index++;
    }

    *(line + index) = '\0';
}

char* mallocAndCopyStr(char* string) {
    if (length(string) > 0) {
        char* str = (char*)malloc(sizeof(char) * length(string));
        memcpy(str, string, length(string) * sizeof(char));
        return str;
    }
    return NULL;
}

void checkAndFree(void* pointer) {
    if (pointer != NULL)
        free(pointer);
}

void printWeatherStructContents(const struct Weather* weather) {
    printf("%s %s %s %d %d\n\n", weather->city, weather->state, weather->gridId, weather->gridX, weather->gridY);

    if (weather->weatherForecast[0].name != NULL) {
        for (int i = 0; i < 14; i++) {
            struct Day day = weather->weatherForecast[i];

            printf("%s Temp: %d°F\n", day.name, day.temperature);

            printf("Detailed Forecast: %s\n", day.detailedForecast);

            printf("\n");
        }
    }

}