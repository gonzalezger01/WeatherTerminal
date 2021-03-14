//constants
const char* LAT_LONG_API = "https://public.opendatasoft.com/api/records/1.0/search/?dataset=us-zip-code-latitude-and-longitude&q=";

//"https://api.weather.gov/gridpoints/TOP/31,80/forecast
const char* WEATHER_API_FORECAST = "https://api.weather.gov/gridpoints/";

//https://api.weather.gov/points/39.7456,-97.0892
const char* WEATHER_API_OFFICE = "https://api.weather.gov/points/";

const char* FORECAST = "forecast";
const char* CONFIG_FOLDER = ".config";
const char* CONFIG_FILE = "weatherConfig.conf";

const char* USER_AGENT = "WeatherTerminal/1.0";

const char* HOME_ENV = "HOME";

//labels
const char* LABEL_SUCCESS_WRITE = "successfully wrote config file!";
const char* LABEL_USER_ENTERED_ZIPCODE = "Entered zipCode %s\n";
const char* LABEL_WELCOME = "Welcome to Weather Terminal\n";
const char* LABEL_ZIPCODE_ENTER = "Please enter a zipCode: ";

const char *HALLOWEEN;
const char *THANKSGIVING;
const char *CHRISTMAS;
const char *NEW_YEARS;

//const values
const int INVALID_TEMP = -100;