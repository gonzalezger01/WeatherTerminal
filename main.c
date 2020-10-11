#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "weather.h"

int doesDirectoryExist();
int trimString(void *memory);
void readFileContents(FILE *fp, void *lp);
void extractValue(void *memory);
void trimNewline(void *memory);
FILE* createConfigFile();


const char *configFolder = ".config";
int main(){
  printf("Welcome to Weather Terminal\n");
  FILE *file;
  char *zipCode;
  struct Location *location = malloc(sizeof(struct Location) * 1);
  
  if(doesDirectoryExist() && (file = fopen("weatherConfig.conf", "r")) != NULL){
    
    printf("File exists\n");
    readFileContents(file, location);
    if(location->lat != NULL && location->lon != NULL)
      printf("Lat %s\nLon %s\n",  location->lat, location->lon);
    
  }
  else{
    file = createConfigFile();
    size_t buffSize = 7;
    zipCode = (char *)  malloc(sizeof(char) * buffSize);
    if(zipCode != NULL){
      printf("Please enter a zipCode: ");
      getline(&zipCode, &buffSize , stdin);
      printf("Entered zipCode %s\n", zipCode);
      trimNewline(zipCode);

      char *destination  = malloc(sizeof(char) * 1);
      if(destination != NULL){
	obtainLatLongData(zipCode, destination);
	free(destination);
      }
      free(zipCode);
    }
  }

  free(location->lat);
  free(location->lon);
  free(location);
  
}

int doesDirectoryExist(){
  chdir(getenv("HOME"));
  chdir(configFolder);
  const size_t dirSize = 50;
  char *currentDir = malloc(sizeof(char)* dirSize);
  getcwd(currentDir, dirSize);
  trimString(currentDir);

  if(currentDir != NULL){
    free(currentDir);
  }else{
    return -1;
  }
  return 1;
}

void readFileContents(FILE *fp, void* lp){
  struct Location *location = (struct Location *) lp;
  if(fp != NULL){
    location->lat = malloc(sizeof(char) * 20);
    location->lon = malloc(sizeof(char) * 20);
    
    if(location->lat != NULL){
      fgets(location->lat, 20, fp);
      extractValue(location->lat);
      trimString(location->lat);
    }
    
    if(location->lon != NULL){
      fgets(location->lon, 20, fp);
      extractValue(location->lon);
      trimString(location->lon);
    }
  }
}

FILE* createConfigFile(){
  printf("Creating file..\n");
  return fopen("weatherConfig.conf", "w");
}

int trimString(void *sp){
  size_t stringSize = 0;
  char *string = (char *)sp;
  while(*(string+stringSize) != '\0'){
    stringSize++;
  }

  string = (char*) realloc(string, sizeof(char) * stringSize + 1);
  return stringSize;
}

void extractValue(void *memory){
  char *string = (char *) memory;
  int counter = 0;
  while(*(string + counter) != '='){
    counter++;
  }
  counter++;

  char *temporaryStorage = malloc(sizeof(char) * 50);
  int index = 0;
  while(*(string + counter + index) != '\0'){
    if(*(string + counter + index) != '\n')
      *(temporaryStorage + index) = *(string + counter + index);
    index++;
  }
  index++;

  memset(string, 0, sizeof(char));
  memcpy(string, temporaryStorage, sizeof(char) * index);
  *(string + index) = '\0';
  free(temporaryStorage);
}

void trimNewline(void *data){
  char *line = (char *) data;
  size_t index = 0;
  while(*(line+index) != '\0'){
    if(*(line+index) == '\n'){
      break;
    }
    index++;
  }

  *(line+index) = '\0';
}
