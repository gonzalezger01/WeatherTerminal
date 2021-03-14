#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "constants.h"
#include "weather.h"
#include "utils.h"

int doesDirectoryExist();
int createConfigFile(const struct Weather* weather);
static void null_test_success(void** state) {
    (void)state; /* unused */
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        /* A test case that does nothing and succeeds. */

        printf("The argument supplied is %s\n", argv[1]);
        const struct CMUnitTest tests[] = {
            cmocka_unit_test(null_test_success),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
    }

    FILE* file;
    bool isLatLongPresent = false;
    struct Weather weather;

    initalizeWeatherStruct(&weather);
    printf("%s", LABEL_WELCOME);
    if (doesDirectoryExist() && (file = fopen(CONFIG_FILE, "r")) != NULL) {
        collectCoordinates(file, &weather);

        printf("Lat:%.5f Lon:%.5f\n", weather.lat, weather.lon);
        isLatLongPresent = true;
    }

    if (!isLatLongPresent) {
        size_t buffSize = 6;
        char* zipCode;
        zipCode = (char*)malloc(sizeof(char) * buffSize);
        if (zipCode != NULL) {
            printf("%s", LABEL_ZIPCODE_ENTER);
            getline(&zipCode, &buffSize, stdin);
            printf(LABEL_USER_ENTERED_ZIPCODE, zipCode);
            trimNewline(zipCode);

            obtainLatLongData(zipCode, &weather);
            printf("Lat:%.5f Lon:=%.5f\n", weather.lat, weather.lon);
            weather.zipCode = mallocAndCopyStr(zipCode);
            free(zipCode);
            if (createlatLonConfigFile(&weather))
                printf("%s", LABEL_SUCCESS_WRITE);
        }
    }
    obtainWeatherOfficeData(&weather);    
    obtainWeatherData(&weather);
    printWeatherStructContents(&weather);
    freeWeatherStruct(&weather);
}

int doesDirectoryExist() {
    chdir(getenv(HOME_ENV));
    chdir(CONFIG_FOLDER);
    const size_t dirSize = 100;
    char* currentDir = calloc(sizeof(char), dirSize);
    if (currentDir != NULL && getcwd(currentDir, dirSize) != NULL) {
        free(currentDir);
        return 1;
    }
    return -1;
}