#include "weather.h"

static int performCurlRequest(void *api, struct Weather *weatherP);
static size_t write_data(void *data, size_t size, size_t nmemb, void *userp);
static void extractZipJSON(void *jsonP, struct Weather *weatherP);

struct Memory {
  char *response;
  size_t size;
};

void obtainLatLongData(void *zip, struct Weather *weatherP){
  char *zipCode = (char *)zip;
  const int zipLength = 6;
  char *API = calloc(sizeof(char), (length(LAT_LONG_API) + zipLength));
  strcat(API, LAT_LONG_API);
  strcat(API, zipCode);
  performCurlRequest(API, weatherP);
  free(API);
}

//most of the code to perform curl request comes from curl's website
static int  performCurlRequest(void *api, struct Weather *weatherP){
  CURL *curl;
  CURLcode res;

  char *url;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if(curl != NULL && api != NULL) {
    url = (char *) api;
    curl_easy_setopt(curl, CURLOPT_URL, url);

    struct Memory mem;
    mem.response = calloc(sizeof(char), 1);
    mem.size = 0;
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &mem);
 
    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
      printf("Error with curl: %d\n", res); 
      return -1;
    }

    printf("Contents %s\n", mem.response);
    extractZipJSON(mem.response, weatherP);
    curl_easy_cleanup(curl);
  }

  return 0;
}

static size_t write_data(void *data, size_t size, size_t nmemb, void *userp){
  size_t realsize = size * nmemb;
  struct Memory *mem = (struct Memory *) userp;

  char *ptr = realloc(mem->response, mem->size + realsize);
   if(ptr == NULL)
     return 0;  /* out of memory! */
 
   mem->response = ptr;
   memcpy(&(mem->response[mem->size]), data, realsize);
   mem->size += realsize;
   mem->response[mem->size] = 0;
 
   return realsize; 
}

static void extractZipJSON(void *jsonP, struct Weather *weatherP){
  char *jsonString = (char *)jsonP;
  cJSON *json = cJSON_Parse(jsonString);

  const cJSON *records = NULL;
  cJSON *record = NULL;
  
  records = cJSON_GetObjectItemCaseSensitive(json, "records");
  cJSON_ArrayForEach(record, records){
    const cJSON *fields = NULL;
    const cJSON *state = NULL;
    const cJSON *city = NULL;
    const cJSON *latitude = NULL;
    const cJSON *longitude = NULL;
    fields = cJSON_GetObjectItemCaseSensitive(record, "fields");
    
    state = cJSON_GetObjectItemCaseSensitive(fields, "state");
    city = cJSON_GetObjectItemCaseSensitive(fields, "city");
    latitude = cJSON_GetObjectItemCaseSensitive(fields, "latitude");
    longitude = cJSON_GetObjectItemCaseSensitive(fields, "longitude");

    if (cJSON_IsString(city) && (city->valuestring != NULL)){
      printf("%s\n", city->valuestring);
      printf("%s\n", state->valuestring);
      printf("%f\n", latitude->valuedouble);
      printf("%f\n", longitude->valuedouble);
    }
  }
}
