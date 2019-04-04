//
// Created by tomas on 02/04/19.
//

#include "catch.hpp"
#include "parsing/operations.h"

TEST_CASE("Test parsing operations") {
    SECTION("IP address operations") {
        WHEN("IP address is set to private or multicast") {
            uint32_t private_ip = 0x0a3e700d; // 10.0.0.0/8
            uint32_t private_ip1 = 0xa9fe12ff; // 169.254.0.0/16
            uint32_t private_ip2 = 0xac1c0003; // 172.16.0.0/12
            uint32_t private_ip3 = 0x7f000035; // 127.0.0.0/8
            uint32_t private_ip4 = 0xe0454545; // 224.0.0.0/4
            uint32_t private_ip5 = 0xc0a801ff; // 192.168.0.0/16
            THEN("IP address should be recognized as not public") {
                CAPTURE(IPAddress::to_string(private_ip));
                REQUIRE(IPAddress::is_public(private_ip) == false);
                CAPTURE(IPAddress::to_string(private_ip1));
                REQUIRE(IPAddress::is_public(private_ip1) == false);
                CAPTURE(IPAddress::to_string(private_ip2));
                REQUIRE(IPAddress::is_public(private_ip2) == false);
                CAPTURE(IPAddress::to_string(private_ip3));
                REQUIRE(IPAddress::is_public(private_ip3) == false);
                CAPTURE(IPAddress::to_string(private_ip4));
                REQUIRE(IPAddress::is_public(private_ip4) == false);
                CAPTURE(IPAddress::to_string(private_ip5));
                REQUIRE(IPAddress::is_public(private_ip5) == false);
            }
        }
        WHEN("IP address is set to public") {
            uint32_t public_ip = 0x08080808; // 8.8.8.8
            uint32_t public_ip1 = 0xc44090f5; // 196.64.144.245
            uint32_t public_ip2 = 0x0b000005; // 11.0.0.5
            THEN("IP address should be recognized as public") {
                CAPTURE(IPAddress::to_string(public_ip));
                REQUIRE(IPAddress::is_public(public_ip) == true);
                CAPTURE(IPAddress::to_string(public_ip1));
                REQUIRE(IPAddress::is_public(public_ip1) == true);
                CAPTURE(IPAddress::to_string(public_ip2));
                REQUIRE(IPAddress::is_public(public_ip2) == true);
            }
        }
    }
}