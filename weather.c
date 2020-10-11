#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "weather.h"
#include "cJSON.h"


static void extractZipJSON();
static int performCurlRequest(void *api,  void *dest);
static size_t write_data(void *data, size_t size, size_t nmemb, void *userp);
static size_t length(void *data);

struct Memory {
  char *response;
  size_t size;
};

char *latLongApi = "https://public.opendatasoft.com/api/records/1.0/search/?dataset=us-zip-code-latitude-and-longitude&q=";


void obtainLatLongData(void *zip, void *dataDest){
  char *zipCode = (char *)zip;
  char *API = malloc(sizeof(char) * (length(latLongApi) + 6));
  strcat(API, latLongApi);
  strcat(API, zipCode);
  performCurlRequest(API, dataDest);
  free(API);
}

//most of the code to perform curl request comes from curl's website
static int performCurlRequest(void *api,  void *dest){
  char *destination = (char *) dest;
  CURL *curl;
  CURLcode res;

  char *url;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if(curl != NULL && dest != NULL && api != NULL) {
    url = (char *) api;
    curl_easy_setopt(curl, CURLOPT_URL, url);

    struct Memory mem;
    mem.response = destination;
    mem.size = 0;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &mem);
 
    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
      printf("Error with curl: %d\n", res); 
      return -1;
    }

    printf("Contents %s\n", mem.response);
    curl_easy_cleanup(curl);
  }

  return 0;
}

static size_t write_data(void *data, size_t size, size_t nmemb, void *userp){
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

//static void extractWeatherJSON(){
//   
//}

void extractZipJSON(void *jsonP, void* dest){
  char *jsonString = (char *)jsonP;
  //char *destination = (char *) dest;
  //  char s[] = "{ \"name\" : \"Jack\", \"age\" : 27 }\0";
  cJSON *json = cJSON_Parse(jsonString);
  char *string = cJSON_Print(json);

  const cJSON *city = NULL;
  const cJSON *state = NULL;
  const cJSON *coordinates = NULL;
  city = cJSON_GetObjectItemCaseSensitive(json, "city");
  state = cJSON_GetObjectItemCaseSensitive(json, "state");
  coordinates = cJSON_GetObjectItemCaseSensitive(json, "coordinates");
  
  if (cJSON_IsString(city) && (name->valuestring != NULL)){
    printf(city->valuestring);
    printf(state->valuestring);
    printf(coordinates->valuestring);
    printf("\n");
  }
}

static size_t length(void *data){
  char *string = (char *)data;
  size_t counter = 0;
  while(*(string+counter) != '\0')
    counter++;
  return counter+1;
}  
