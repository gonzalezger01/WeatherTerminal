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


struct Location {
  char *lat;
  char *lon;
};

const char *configFolder = ".config";
int main(){
  printf("Welcome to Weather Terminal\n");
  FILE *file;
  char *zipCode;
  
  if(doesDirectoryExist() && (file = fopen("weatherConfig.conf", "r")) != NULL){
    
    printf("File exists\n");
    struct Location *location = malloc(sizeof(struct Location) * 1);
    readFileContents(file, location);

    if(location->lat != NULL && location->lon != NULL)
      printf("Lat %s\nLon %s\n",  location->lat, location->lon);
    
  }
  else{
    file = createConfigFile();
    zipCode = (char *)  malloc(sizeof(char) * 6);
    size_t buffSize = 6;
    printf("Please enter a zipCode: ");
    getline(&zipCode, &buffSize , stdin);
    printf("Entered zipCode %s\n", zipCode);
    trimNewline(zipCode);
  }
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

void trimNewline(void *memory){
  char* string = (char *) memory;
  int index = 0;
  while(*(string + index) != '\0'){
    if(*(string + index) == '\n')
      break;
    index++;
  }
  
  string = realloc(string, sizeof(char) * index+1);
  *(string+index+1) = '\0';
}
