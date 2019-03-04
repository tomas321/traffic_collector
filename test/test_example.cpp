//
// Created by tomas on 03/03/19.
//

#include "catch.hpp"

TEST_CASE(){
    auto x = GENERATE( as<std::string>(), "one", "two", "three" );
    SECTION("Test string length") {
        WHEN("strings are of max length 5") {
            THEN("string length should be less than 6"){
                CAPTURE(x.length());
                CHECK( x.length() < 4 );
            }
        }
    }
}