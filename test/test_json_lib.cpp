#include "catch.hpp"
#include "json.h"

TEST_CASE("Test JSON parsing") {
    Json json;

    SECTION("SImple JSON") {
        WHEN("A simple JSON string is created") {
            json.add<string>("name", "Tomas");
            json.add<int>("age", 22);


            THEN("It should match the expectation") {
                REQUIRE_THAT(json.stringify(), Catch::Matchers::Equals("{\"name\":\"Tomas\",\"age\":22}"));
            }
        }
    }
}