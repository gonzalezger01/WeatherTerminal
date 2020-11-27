#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "constants.h"
#include "weather.h"
#include "utils.h"

int doesDirectoryExist();
int trimString(char *string);
void readFileContents(FILE *fp, struct Weather *lp);
void extractValue(char *string);
void trimNewline(char *string);
char * collectDimension(char *dest, size_t size, FILE *fp);
FILE* createConfigFile();

int main(){
  FILE *file;
  char *zipCode;
  bool isWeatherDataPresent = false; 
  struct Weather weather;
  struct Weather *weatherP = &weather;

  printf("Welcome to Weather Terminal\n");
  if(doesDirectoryExist() && (file = fopen(CONFIG_FILE, "r")) != NULL){
    readFileContents(file, weatherP);
    fclose(file);

    if(weather.lat != NULL && weather.lon != NULL){
      printf("Lat%s\nLon%s\n",  weather.lat, weather.lon);
      isWeatherDataPresent = true;
    }
  }

  if(!isWeatherDataPresent){
    file = createConfigFile();
    size_t buffSize = 0;
    zipCode = (char *)  malloc(sizeof(char) * buffSize);
    if(zipCode != NULL){
      printf("Please enter a zipCode: ");
      getline(&zipCode, &buffSize , stdin);
      printf("Entered zipCode %s\n", zipCode);
      trimNewline(zipCode);

      obtainLatLongData(zipCode, weatherP);
      free(zipCode);
    }
  }
  
    if(weather.lat != NULL)
      free(weather.lat);
    if(weather.lon != NULL)
      free(weather.lon);
}

int doesDirectoryExist(){
  chdir(getenv("HOME"));
  chdir(CONFIG_FOLDER);
  const size_t dirSize = 50;
  char *currentDir = calloc(sizeof(char), dirSize);
  getcwd(currentDir, dirSize);
  trimString(currentDir);

  if(currentDir != NULL){
    free(currentDir);
  }else{
    return -1;
  }
  return 1;
}

void readFileContents(FILE *fp, struct Weather* weather){
  if(fp != NULL){
    size_t dimensionLength = 20;
    char *lat = calloc(sizeof(char), dimensionLength);
    char *lon = calloc(sizeof(char), dimensionLength);

    if(lat != NULL && lon != NULL){
      weather->lat = collectDimension(lat, dimensionLength, fp);
      weather->lon =  collectDimension(lon, dimensionLength, fp);
    }
    
  }
}

FILE* createConfigFile(){
  printf("Creating file..\n");
  return fopen("weatherConfig.conf", "w");
}

void extractValue(char *string){
  int counter = 0;
  
  while(*(string + counter) != '='){
    counter++;
  }
  
  char *temporaryStorage = calloc(sizeof(char), 50);
  int index = 0;
  while(*(string + counter + index) != '\0'){
    if(*(string + counter + index) != '\n')
      *(temporaryStorage + index) = *(string + counter + index);
    index++;
  }
  index++;

  memcpy(string, temporaryStorage, sizeof(char) * index);
  *(string + index) = '\0';
  free(temporaryStorage);
}

int trimString(char *string){
  size_t stringSize = 0;
  while(*(string+stringSize) != '\0'){
    stringSize++;
  }

  char *trimmedString = (char*) realloc(string, sizeof(char) * stringSize + 1);
  *string = *trimmedString;
  return stringSize;
}

void trimNewline(char *line){
  size_t index = 0;
  while(*(line+index) != '\0'){
    if(*(line+index) == '\n'){
      break;
    }
    index++;
  }

  *(line+index) = '\0';
}

char * collectDimension(char *dest, size_t size, FILE *fp){
  if(fgets(dest, size, fp) != NULL){
    if(*dest != '\0'){
      extractValue(dest);
      trimString(dest);
    }else{
      free(dest);
      dest = NULL;
    }
  }else{
    perror("fgets() failed");
    free(dest);
    dest = NULL;
  }

  return dest;
}
