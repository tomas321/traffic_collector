#include <GeoIP.h>

int main() {
    FILE *f;
    char *db_info;
    char ipAddress[30];
    char expectedCountry[3];
    char expectedCountry3[4];
    const char *returnedCountry;
    GeoIP *gi;
    int failed = 0;
    int test_num = 1;

    int i;
    for (i = 0; i < 2; ++i) {
        if (0 == i) {
            /* Read from filesystem, check for updated file */
            gi = GeoIP_open("resources/geoip/GeoIP.dat",
                    GEOIP_STANDARD | GEOIP_CHECK_CACHE);
        } else {
            /* Read from memory, faster but takes up more memory */
            gi = GeoIP_open("resources/geoip/GeoIP.dat", GEOIP_MEMORY_CACHE);
        }

        if (gi == NULL) {
            fprintf(stderr, "Error opening database\n");
            exit(1);
        }

        db_info = GeoIP_database_info(gi);
        if (!db_info || strcmp(db_info,
                               "GEO-106FREE 20180327 Build 1 Copyright (c) 2018 "
                               "MaxMind Inc All Rights Reserved")) {
            fprintf(stderr, "Error reading database info (got %s).\n", db_info);
            free(db_info);
            failed = 1;
        }

        /* make sure GeoIP deals with invalid query gracefully */
        returnedCountry = GeoIP_country_code_by_addr(gi, NULL);
        if (returnedCountry != NULL) {
            fprintf(stderr,
                    "Invalid Query test failed, got non NULL, expected NULL\n");
            failed = 1;
        }

        returnedCountry = GeoIP_country_code_by_name(gi, NULL);
        if (returnedCountry != NULL) {
            fprintf(stderr,
                    "Invalid Query test failed, got non NULL, expected NULL\n");
            failed = 1;
        }

        f = fopen("test/country_test.txt", "r");

        while (
                fscanf(f, "%s%s%s", ipAddress, expectedCountry, expectedCountry3) !=
                EOF) {
            returnedCountry = GeoIP_country_code_by_addr(gi, ipAddress);
            if (returnedCountry == NULL ||
                strcmp(returnedCountry, expectedCountry) != 0) {
                fprintf(stderr,
                        "Test addr %d for %s failed, got %s, expected %s\n",
                        test_num,
                        ipAddress,
                        returnedCountry,
                        expectedCountry);
                failed = 1;
            }
            returnedCountry = GeoIP_country_code_by_name(gi, ipAddress);
            if (returnedCountry == NULL ||
                strcmp(returnedCountry, expectedCountry) != 0) {
                fprintf(stderr,
                        "Test name %d for %s failed, got %s, expected %s\n",
                        test_num,
                        ipAddress,
                        returnedCountry,
                        expectedCountry);
                failed = 1;
            }
            returnedCountry = GeoIP_country_code3_by_addr(gi, ipAddress);
            if (returnedCountry == NULL ||
                strcmp(returnedCountry, expectedCountry3) != 0) {
                fprintf(stderr,
                        "Test addr %d for %s failed, got %s, expected %s\n",
                        test_num,
                        ipAddress,
                        returnedCountry,
                        expectedCountry);
                failed = 1;
            }
            returnedCountry = GeoIP_country_code3_by_name(gi, ipAddress);
            if (returnedCountry == NULL ||
                strcmp(returnedCountry, expectedCountry3) != 0) {
                fprintf(stderr,
                        "Test name %d for %s failed, got %s, expected %s\n",
                        test_num,
                        ipAddress,
                        returnedCountry,
                        expectedCountry);
                failed = 1;
            }
            test_num++;
        }
        fclose(f);

        f = fopen("test/country_test2.txt", "r");
        while (fscanf(f, "%s%s", ipAddress, expectedCountry) != EOF) {
            returnedCountry = GeoIP_country_code_by_addr(gi, ipAddress);
            if (returnedCountry == NULL ||
                strcmp(returnedCountry, expectedCountry) != 0) {
                fprintf(stderr,
                        "Test addr %d %s failed, got %s, expected %s\n",
                        test_num,
                        ipAddress,
                        returnedCountry,
                        expectedCountry);
                failed = 1;
            }
            test_num++;
        }
        fclose(f);
        GeoIP_delete(gi);
    }
    return failed;
}
