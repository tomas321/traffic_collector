//
// Created by tomas on 03/03/19.
//

#include <iostream>
#include "catch.hpp"
#include "parsing/operations.h"
#include "parsing/layers.h"

TEST_CASE ("Test Ethernet header parsing") {
    uint8_t x[] = {
            0x2c, 0x30, 0x33, 0xf9, 0x60, 0xc0, 0xac, 0xe2,
            0xd3, 0x5a, 0x30, 0x08, 0x08, 0x00
    };

    SECTION ("Check Ethernet layer") {
        WHEN ("Header data is parsed") {
            auto eth = Ethernet::parse(x);
            THEN ("Ethertype should match") {
                REQUIRE_THAT(Packet::supported_layers.at(static_cast<Packet::Type >(eth->ethertype)), Catch::Matchers::Equals("ipv4", Catch::CaseSensitive::No));
            }
            THEN ("Destination MAC should match") {
                REQUIRE_THAT(MACAddress::to_string(eth->dst_mac), Catch::Matchers::Equals("2C:30:33:F9:60:C0", Catch::CaseSensitive::No));
            }
            THEN ("Source MAC should match") {
                REQUIRE_THAT(MACAddress::to_string(eth->src_mac), Catch::Matchers::Equals("AC:E2:D3:5A:30:08", Catch::CaseSensitive::No));
            }
        }
    }
}

TEST_CASE ("Test IPv4 header parsing") {
    uint8_t x[] = {
            0x45, 0x00, 0x01, 0x5f, 0x81, 0xaa, 0x40, 0x00,
            0x40, 0x06, 0x3a, 0xab, 0x0a, 0x00, 0x00, 0x2f,
            0x1f, 0x0d, 0x54, 0x08
    };

    SECTION ("Check Ethernet layer") {
        WHEN ("Header data is parsed") {
            auto ip = IPv4::parse(x);
            THEN ("Version should match") {
                REQUIRE(ip->version == 0x04);
            }
            THEN ("Header length should match") {
                REQUIRE(ip->ihl == 0x05);
            }
            THEN ("Type of service should match") {
                REQUIRE(ip->tos == 0x00);
            }
            THEN ("Total length should match") {
                REQUIRE(ip->total_length == 0x015f);
            }
            THEN ("Identification should match") {
                REQUIRE(ip->identification == 0x81aa);
            }
            THEN ("Flags should match") {
                REQUIRE(ip->flags == 0x02); // last 3 bits
            }
            THEN ("Offset should match") {
                REQUIRE(ip->offset == 0x0000);
            }
            THEN ("TTL should match") {
                REQUIRE(ip->ttl == 0x40);
            }
            THEN ("Protocol should match") {
                REQUIRE(ip->protocol == 0x06);
            }
            THEN ("Checksum should match") {
                REQUIRE(ip->hdr_checksum == 0x3aab);
            }
            THEN ("Source IP should match") {
                CAPTURE(ip->src_ip);
                REQUIRE_THAT(IPAddress::to_string(ip->src_ip), Catch::Matchers::Equals("10.0.0.47"));
            }
            THEN ("Destination IP should match") {
                REQUIRE_THAT(IPAddress::to_string(ip->dst_ip), Catch::Matchers::Equals("31.13.84.8"));
            }
        }
    }
}