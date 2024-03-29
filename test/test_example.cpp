#include <iostream>
#include "catch.hpp"

#include "parsing/icmp.h"
#include "parsing/icmpv6.h"
#include "parsing/tcp.h"
#include "parsing/udp.h"
#include "parsing/dccp.h"
#include "parsing/arp.h"
#include "parsing/ipv4.h"
#include "parsing/ipv6.h"
#include "parsing/ethernet.h"
#include "parsing/operations.h"

TEST_CASE ("Test Ethernet header parsing") {
    uint8_t x[] = {
            0x2c, 0x30, 0x33, 0xf9, 0x60, 0xc0, 0xac, 0xe2,
            0xd3, 0x5a, 0x30, 0x08, 0x08, 0x00
    };

    SECTION ("Check Ethernet layer") {
        WHEN ("Header data is parsed") {
            auto eth = new Ethernet(x);
            THEN ("Ethertype should match") {
                REQUIRE_THAT(Layers::layer_string(static_cast<Layers::Type >(eth->header->ethertype)),
                             Catch::Matchers::Equals("ipv4", Catch::CaseSensitive::No));
            }
            THEN ("Destination MAC should match") {
                REQUIRE_THAT(MACAddress::to_string(eth->header->dst_mac),
                             Catch::Matchers::Equals("2C:30:33:F9:60:C0", Catch::CaseSensitive::No));
            }
            THEN ("Source MAC should match") {
                REQUIRE_THAT(MACAddress::to_string(eth->header->src_mac),
                             Catch::Matchers::Equals("AC:E2:D3:5A:30:08", Catch::CaseSensitive::No));
            }
        }
    }
}

TEST_CASE ("Test ARP header parsing") {
    uint8_t request[] = {
            0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01,
            0x54, 0x67, 0x51, 0x6f, 0x48, 0x4b, 0xc0, 0xa8,
            0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xc0, 0xa8, 0x00, 0xba
    };
    uint8_t response[] = {
            0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02,
            0xf8, 0x94, 0xc2, 0x1a, 0x1d, 0xeb, 0xc0, 0xa8,
            0x00, 0xba, 0x54, 0x67, 0x51, 0x6f, 0x48, 0x4b,
            0xc0, 0xa8, 0x00, 0x01
    };

    SECTION ("Check ARP request layer") {
        WHEN ("Header data is parsed") {
            auto arp = new ARP(request);
            THEN ("HW type should match") {
                REQUIRE(arp->header->hw_type == 0x0001);
            }
            THEN ("Protocol type should match") {
                REQUIRE(arp->header->ip_type == 0x0800);
            }
            THEN ("HW address length should match") {
                REQUIRE(arp->header->hwaddr_len == 0x06);
            }
            THEN ("Protocol address length should match") {
                REQUIRE(arp->header->l3addr_len == 0x04);
            }
            THEN ("Operation code should match") {
                REQUIRE(arp->header->opcode == 0x0001);
            }
            THEN ("Sender MAC address should match") {
                REQUIRE_THAT(MACAddress::to_string(arp->header->sender_mac),
                             Catch::Matchers::Equals("54:67:51:6f:48:4b", Catch::CaseSensitive::No));
            }
            THEN ("Sender Protocol address should match") {
                REQUIRE_THAT(IPAddress::to_string(arp->header->sender_ip), Catch::Matchers::Equals("192.168.0.1"));
            }
            THEN ("Target MAC address should match") {
                REQUIRE_THAT(MACAddress::to_string(arp->header->target_mac), Catch::Matchers::Equals("00:00:00:00:00:00"));
            }
            THEN ("Target Protocol address should match") {
                REQUIRE_THAT(IPAddress::to_string(arp->header->target_ip), Catch::Matchers::Equals("192.168.0.186"));
            }
        }
    }

    SECTION ("Check ARP request layer") {
        WHEN ("Header data is parsed") {
            auto arp = new ARP(response);
            THEN ("HW type should match") {
                REQUIRE(arp->header->hw_type == 0x0001);
            }
            THEN ("Protocol type should match") {
                REQUIRE(arp->header->ip_type == 0x0800);
            }
            THEN ("HW address length should match") {
                REQUIRE(arp->header->hwaddr_len == 0x06);
            }
            THEN ("Protocol address length should match") {
                REQUIRE(arp->header->l3addr_len == 0x04);
            }
            THEN ("Operation code should match") {
                REQUIRE(arp->header->opcode == 0x0002);
            }
            THEN ("Sender MAC address should match") {
                REQUIRE_THAT(MACAddress::to_string(arp->header->sender_mac),
                             Catch::Matchers::Equals("f8:94:c2:1a:1d:eb", Catch::CaseSensitive::No));
            }
            THEN ("Sender Protocol address should match") {
                REQUIRE_THAT(IPAddress::to_string(arp->header->sender_ip), Catch::Matchers::Equals("192.168.0.186"));
            }
            THEN ("Target MAC address should match") {
                REQUIRE_THAT(MACAddress::to_string(arp->header->target_mac),
                             Catch::Matchers::Equals("54:67:51:6f:48:4b", Catch::CaseSensitive::No));
            }
            THEN ("Target Protocol address should match") {
                REQUIRE_THAT(IPAddress::to_string(arp->header->target_ip), Catch::Matchers::Equals("192.168.0.1"));
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

    SECTION ("Check IPv4 layer") {
        WHEN ("Header data is parsed") {
            auto ip = new IPv4(x);
            THEN ("Version should match") {
                REQUIRE(ip->header->version == 0x04);
            }
            THEN ("Header length should match") {
                REQUIRE(ip->header->ihl == 0x05);
            }
            THEN ("Type of service should match") {
                REQUIRE(ip->header->tos == 0x00);
            }
            THEN ("Total length should match") {
                REQUIRE(ip->header->total_length == 0x015f);
            }
            THEN ("Identification should match") {
                REQUIRE(ip->header->identification == 0x81aa);
            }
            THEN ("Flags should match") {
                REQUIRE(ip->header->flags == 0x02); // last 3 bits
            }
            THEN ("Offset should match") {
                REQUIRE(ip->header->offset == 0x0000);
            }
            THEN ("TTL should match") {
                REQUIRE(ip->header->ttl == 0x40);
            }
            THEN ("Protocol should match") {
                REQUIRE(ip->header->protocol == 0x06);
            }
            THEN ("Checksum should match") {
                REQUIRE(ip->header->hdr_checksum == 0x3aab);
            }
            THEN ("Source IP should match") {
                CAPTURE(ip->header->src_ip);
                REQUIRE_THAT(IPAddress::to_string(ip->header->src_ip), Catch::Matchers::Equals("10.0.0.47"));
            }
            THEN ("Destination IP should match") {
                REQUIRE_THAT(IPAddress::to_string(ip->header->dst_ip), Catch::Matchers::Equals("31.13.84.8"));
            }
        }
    }
}

TEST_CASE ("Test ICMP header parsing") {
    uint8_t x[] = {
            0x08, 0x00, 0xf1, 0x47, 0x26, 0x13, 0x00, 0x02,
            0x1b, 0x24, 0x95, 0x5c, 0x00, 0x00, 0x00, 0x00,
            0x69, 0x4f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
            0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
            0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
            0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
    };

    SECTION ("Check ICMP layer") {
        WHEN ("Header data is parsed") {
            auto icmp = new ICMP(x);
            THEN ("Type should match") {
                REQUIRE(icmp->header->type == 0x08);
            }
            THEN ("Code should match") {
                REQUIRE(icmp->header->code == 0x00);
            }
            THEN ("Header checksum should match") {
                REQUIRE(icmp->header->hdr_checksum == 0xf147);
            }
        }
    }
}

TEST_CASE ("Test ICMPv6 header parsing") {
    uint8_t x[] = {
            0x08, 0x00, 0xf1, 0x47, 0x26, 0x13, 0x00, 0x02,
            0x1b, 0x24, 0x95, 0x5c, 0x00, 0x00, 0x00, 0x00,
            0x69, 0x4f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
            0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
            0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
            0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
    };

    SECTION ("Check ICMPv6 layer") {
        WHEN ("Header data is parsed") {
            auto icmp = new ICMPv6(x);
            THEN ("Type should match") {
                REQUIRE(icmp->header->type == 0x08);
            }
            THEN ("Code should match") {
                REQUIRE(icmp->header->code == 0x00);
            }
            THEN ("Header checksum should match") {
                REQUIRE(icmp->header->hdr_checksum == 0xf147);
            }
        }
    }
}

TEST_CASE ("Test UDP header parsing") {
    uint8_t x[] = {
            0xbd, 0x57, 0x00, 0x35, 0x00, 0x29, 0x08, 0x36,
            0x85, 0x69, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x03, 0x73, 0x73, 0x6c,
            0x07, 0x67, 0x73, 0x74, 0x61, 0x74, 0x69, 0x63,
            0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00,
            0x01
    };

    SECTION ("Check UDP layer") {
        WHEN ("Header data is parsed") {
            auto udp = new UDP(x);
            THEN ("Source port should match") {
                REQUIRE(udp->header->src_port == 0xbd57);
            }
            THEN ("Destination port should match") {
                REQUIRE(udp->header->dst_port == 0x0035);
            }
            THEN ("Datagram length should match") {
                REQUIRE(udp->header->length == 0x0029);
            }
            THEN ("Header checksum should match") {
                REQUIRE(udp->header->checksum == 0x0836);
            }
//            THEN ("Data should match") {
//                REQUIRE_THAT(udp->header->data, Catch::Matchers::Equals("..."));
//            }
        }
    }
}

TEST_CASE ("Test TCP header parsing") {
    uint8_t x[] = {
            0xa8, 0x9e, 0x01, 0xbb, 0x0e, 0x92, 0x91, 0x9e,
            0xa9, 0x72, 0xe7, 0x71, 0x80, 0x10, 0x01, 0x43,
            0x52, 0xe0, 0x00, 0x00, 0x01, 0x01, 0x08, 0x0a,
            0xb2, 0xdb, 0x13, 0x9b, 0x20, 0x95, 0x37, 0x99
    };

    SECTION ("Check TCP layer") {
        WHEN ("Header data is parsed") {
            auto tcp = new TCP(x);
            THEN ("Source port should match") {
                REQUIRE(tcp->header->src_port == 0xa89e);
            }
            THEN ("Destination port should match") {
                REQUIRE(tcp->header->dst_port == 0x01bb);
            }
            THEN ("Sequence number should match") {
                REQUIRE(tcp->header->seq == 0x0e92919e);
            }
            THEN ("Acknowledgement number should match") {
                REQUIRE(tcp->header->ack == 0xa972e771);
            }
            THEN ("Offset should match") {
                REQUIRE(tcp->header->offset == 0x08);
            }
            THEN ("Control bits should match") {
                REQUIRE(tcp->header->control_bits == 0x10);
            }
            THEN ("Window should match") {
                REQUIRE(tcp->header->window == 0x0143);
            }
            THEN ("Checksum should match") {
                REQUIRE(tcp->header->checksum == 0x52e0);
            }
            THEN ("Urgent pointer should match") {
                REQUIRE(tcp->header->urgent_ptr == 0x0000);
            }
        }
    }
}

TEST_CASE ("Test IPv6 header parsing") {
    uint8_t x[] = {
            0x60, 0x04, 0xeb, 0xed, 0x00, 0x95, 0x11, 0xff,
            0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xae, 0x8f, 0x27, 0x94, 0x6e, 0xe1, 0x42, 0x4a,
            0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb
    };


    SECTION ("Check IPv6 layer") {
        WHEN ("Header data is parsed") {
            auto ip = new IPv6(x);
            THEN ("Version should match") {
                REQUIRE(ip->header->version == 0x06);
            }
            THEN ("Traffic class should match") {
                REQUIRE(ip->header->traffic_class == 0x00);
            }
            THEN ("Flow label should match") {
                REQUIRE(ip->header->flow_label == 0x0004ebed);
            }
            THEN ("Payload length should match") {
                REQUIRE(ip->header->payload_len == 0x0095);
            }
            THEN ("Next header should match") {
                REQUIRE(ip->header->next_hdr == 0x11);
            }
            THEN ("Hop limit should match") {
                REQUIRE(ip->header->hop_limit == 0xff);
            }
            THEN ("Source IP should match") {
                REQUIRE_THAT(IPv6Address::to_string(ip->header->src_ip), Catch::Matchers::Equals("fe80:0000:0000:0000:ae8f:2794:6ee1:424a", Catch::CaseSensitive::No));
            }
            THEN ("Destination IP should match") {
                REQUIRE_THAT(IPv6Address::to_string(ip->header->dst_ip), Catch::Matchers::Equals("ff02:0000:0000:0000:0000:0000:0000:00fb", Catch::CaseSensitive::No));
            }
        }
    }
}

TEST_CASE("Test DCCP header parsing") {
    uint8_t x[] = {
            0x80, 0x04, 0x13, 0x89, 0x05, 0x00, 0x08, 0xdb,
            0x01, 0x00, 0x00, 0x04, 0x29, 0x01, 0x6d, 0xdc
    };

    uint8_t xx[] = {
            0x80, 0x04, 0x13, 0x89, 0x05, 0x00, 0x08, 0xdb,
            0x00, 0x00, 0x00, 0x04
    };



    SECTION("Check DCCP layer") {
        WHEN("Header data is parsed") {
            auto dccp = new DCCP(x);
            THEN("Source port should match") {
                REQUIRE(dccp->header->src_port == 0x8004);
            }
            THEN("Destination port should match") {
                REQUIRE(dccp->header->dst_port == 0x1389);
            }
            THEN("Offset should match") {
                REQUIRE(dccp->header->offset == 0x05);
            }
            THEN("CCID value should match") {
                REQUIRE(dccp->header->ccval == 0x00);
            }
            THEN("Checksum coverage should match") {
                REQUIRE(dccp->header->checksum_cov == 0x00);
            }
            THEN("Checksum should match") {
                REQUIRE(dccp->header->checksum == 0x08db);
            }
            THEN("Type should match") {
                REQUIRE(dccp->header->type == 0x00);
            }
            THEN("Extended seq number should match") {
                REQUIRE(dccp->header->x == 0x01);
            }
            THEN("Sequence number should match") {
                REQUIRE(dccp->header->seq == 17867828700);
            }
        }
        WHEN("Header data is parsed") {
            auto dccp2 = new DCCP(xx);
            THEN("Source port should match") {
                REQUIRE(dccp2->header->src_port == 0x8004);
            }
            THEN("Destination port should match") {
                REQUIRE(dccp2->header->dst_port == 0x1389);
            }
            THEN("Offset should match") {
                REQUIRE(dccp2->header->offset == 0x05);
            }
            THEN("CCID value should match") {
                REQUIRE(dccp2->header->ccval == 0x00);
            }
            THEN("Checksum value should match") {
                REQUIRE(dccp2->header->checksum_cov == 0x00);
            }
            THEN("Checksum should match") {
                REQUIRE(dccp2->header->checksum == 0x08db);
            }
            THEN("Type should match") {
                REQUIRE(dccp2->header->type == 0x00);
            }
            THEN("Extended seq number should match") {
                REQUIRE(dccp2->header->x == 0x00);
            }
            THEN("Sequence number should match") {
                REQUIRE(dccp2->header->seq == 4);
            }
        }
    }
}