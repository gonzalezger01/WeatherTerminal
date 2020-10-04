#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "weather.h"
#include "cJSON.h"

struct Memory {
  char *response;
  size_t size;
};

char *latLongApi = "https://public.opendatasoft.com/api/records/1.0/search/?dataset=us-zip-code-latitude-and-longitude&q=";

//most of the code to perform curl request comes from curl's website
int performCurlRequest(void *api,  void *dest){
  char *destination = (char *) dest;
  CURL *curl;
  CURLcode res;

  char *url;
  curl = curl_easy_init();
  if(curl && dest != NULL && api != NULL) {
    if(api != NULL)
      url = api;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    destination = malloc(sizeof(char) * 1);
    struct Memory *mem = malloc(sizeof(struct Memory) * 1);
    mem->response = destination;
    mem->size = 0;
    
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem);
    printf("Contents %s", mem->response);
 
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      return -1;
 
    curl_easy_cleanup(curl);
    free(mem);
  }

  return 0;
}

size_t write_data(void *data, size_t size, size_t nmemb, void *userp){
  size_t realsize = size * nmemb;
  struct Memory *mem = (struct Memory *) userp;

  char *ptr = realloc(mem->response, mem->size + realsize + 1);
   if(ptr == NULL)
     return 0;  /* out of memory! */
 
   mem->response = ptr;
   memcpy(&(mem->response[mem->size]), data, realsize);
   mem->size += realsize;
   mem->response[mem->size] = 0;
 
   return realsize;
  
}

void extractWeatherJSON(){
   
}

void extractZipJSON(){
  char s[] = "{ \"name\" : \"Jack\", \"age\" : 27 }\0";
  cJSON *json = cJSON_Parse(s);
  char *string = cJSON_Print(json);

  const cJSON *name = NULL;
  name = cJSON_GetObjectItemCaseSensitive(json, "name");
  
  if (cJSON_IsString(name) && (name->valuestring != NULL)){
    printf(name->valuestring);
    printf("\n");
  }

  printf("%s %s\n", string, name->valuestring);
}

void obtainLatLongData(void *zip, void *dataDest){
  char *zipCode = (char *)zip;
  latLongApi = realloc(latLongApi, sizeof(char) * (length(latLongApi)+ 6));
  latLongApi = strcat(latLongApi, zipCode);
  performCurlRequest(latLongApi, dataDest);
  char *dataDestination = (char *)dataDest;
  printf("Zip data %s\n", dataDestination); 
}

size_t length(void *data){
  char *string = (char *)data;
  size_t counter = 0;
  while(*(string+counter) != '\0')
    counter++;
  return counter+1;
}  
