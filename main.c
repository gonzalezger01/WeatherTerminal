#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#Include "constants.h"
#include "weather.h"

int doesDirectoryExist();
int trimString(char **memory);
void readFileContents(FILE *fp, struct Weather** lp);
void extractValue(char **memory);
void trimNewline(void *memory);
FILE* createConfigFile();

int main(){
  printf("Welcome to Weather Terminal\n");
  FILE *file;
  char *zipCode;
  struct Location *location = NULL;
  
  if(doesDirectoryExist() && (file = fopen(CONFIG_FILE, "r")) != NULL){
    
    printf("File exists\n");
    readFileContents(file, &location);
    fclose(file);
    if(location->lat != NULL && location->lon != NULL)
      printf("Lat%s\nLon%s\n",  location->lat, location->lon);
    
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

      if(destination != NULL){
	obtainLatLongData(zipCode, &weatherP);
	free(destination);
      }
      free(zipCode);
    }
  }

  if(location != NULL){
    if(location->lat != NULL)
      free(location->lat);
    if(location->lon != NULL)
      free(location->lon);
    free(location);
  }
  
}

int doesDirectoryExist(){
  chdir(getenv("HOME"));
  chdir(CONFIG_FOLDER);
  const size_t dirSize = 50;
  char *currentDir = calloc(sizeof(char), dirSize);
  getcwd(currentDir, dirSize);
  trimString(&currentDir);

  if(currentDir != NULL){
    free(currentDir);
  }else{
    return -1;
  }
  return 1;
}

void readFileContents(FILE *fp, struct Location** location){
  *location = calloc(sizeof(struct Location), 1);
  if(fp != NULL){
    (*location)->lat = calloc(sizeof(char), 20);
    (*location)->lon = calloc(sizeof(char), 20);
    
    if((*location)->lat != NULL){
      fgets((*location)->lat, 20, fp);
      extractValue(&(*location)->lat);
      trimString(&(*location)->lat);
    }
    
    if((*location)->lon != NULL){
      fgets((*location)->lon, 20, fp);
      extractValue(&(*location)->lon);
      trimString(&(*location)->lon);
    }
  }
}

FILE* createConfigFile(){
  printf("Creating file..\n");
  return fopen("weatherConfig.conf", "w");
}

int trimString(char **string){
  size_t stringSize = 0;
  while(*(*string+stringSize) != '\0'){
    stringSize++;
  }

  char *trimmedString = (char*) realloc(*string, sizeof(char) * stringSize + 1);
  *string = trimmedString;
  return stringSize;
}

void extractValue(char **string){
  int counter = 0;
  while(*((*string) + counter) != '='){
    counter++;
  }

  char *temporaryStorage = calloc(sizeof(char), 50);
  int index = 0;
  while(*(*string + counter + index) != '\0'){
    if(*(*string + counter + index) != '\n')
      *(temporaryStorage + index) = *(*string + counter + index);
    index++;
  }
  index++;

  memset(*string, 0, sizeof(char));
  memcpy(*string, temporaryStorage, sizeof(char) * index);
  *(*string + index) = '\0';
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
