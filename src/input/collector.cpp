//
// Created by tomas on 10/03/19.
//

#include "collector.h"
#include "logging.h"
#include "exceptions.h"

#include <linux/if_packet.h>
#include <parser.h>


Collector::Collector(const sensor_settings &sensor_config, const filter_settings &filter_config) : sensor_config(
        sensor_config), filter_config(filter_config) {
    device_ip_addr = "";
    device_mac_addr = "";
    setup_handle();

//    capture_network_packets(10);
}

void Collector::setup_handle() {
    Logging::log(info, "setting up capture handle on " + sensor_config.interface);
    set_capture_handle(sensor_config.interface);
    set_preferences();
    activate_handle();
}

int Collector::set_capture_handle(const string &device) {
    pcap_if_t *alldevs;
    char errbuff[PCAP_ERRBUF_SIZE];
    unsigned char *mac;

    if (pcap_findalldevs(&alldevs, errbuff) == PCAP_ERROR) {
        Logging::log(fatal, "Couldn't find any available device to capture on");
        throw CaptureError("No available device");
    }

    for (pcap_if_t *current = alldevs; current != NULL; current = current->next) {
        if (string(current->name) == device) {
            Logging::log(debug, "Found " + device);
            for (pcap_addr_t *address = current->addresses; address != NULL; address = address->next) {
                switch (address->addr->sa_family) {
                    case PF_PACKET:
                        char mac_addr[18];
                        mac = ((struct sockaddr_ll *) address->addr)->sll_addr;
                        snprintf(mac_addr, sizeof(mac_addr), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                        device_mac_addr = string(mac_addr);

                        Logging::log(debug, string(current->name) + ": mac address: " + string(device_mac_addr));
                        break;
                    case AF_INET:
                        device_ip_addr = string(inet_ntoa(((struct sockaddr_in *) address->addr)->sin_addr));
                        Logging::log(debug, string(current->name) + ": ip address: " + string(device_ip_addr));
                        break;
                    default:
                        break;
                }
            }
            if (device_ip_addr.empty() && device_mac_addr.empty()) {
                throw CaptureError("No addresses for selected device: " + device, error);
            } else {
                capture_handle = pcap_create(current->name, errbuff);
                Logging::log(debug, "Initialized pcap capture handle for: '" + string(current->name) + "'");
                pcap_freealldevs(alldevs);
                return 0;
            }
        }
    }
    throw CaptureError(device + " is not available capture device");
}

int Collector::set_preferences() {
    int set_direction;
    Logging::log(info, "Setting preferences for capture handle");

    if (pcap_set_snaplen(capture_handle, SNAPLEN) != 0)
        Logging::log(warning, "Cannot set specified snaplen (" + to_string(SNAPLEN) + ")");
    if (pcap_set_immediate_mode(capture_handle, 1) != 0)
        Logging::log(warning, "Cannot set immediate mode");
    if (pcap_set_buffer_size(capture_handle, GIGABIT_SEC_SIZE) != 0)
        Logging::log(warning, "Cannot set specified buffer size (" + to_string(GIGABIT_SEC_SIZE) + ")");
    if (pcap_set_tstamp_type(capture_handle, PCAP_TSTAMP_ADAPTER) != 0) {
        string ts_type(pcap_tstamp_type_val_to_name(PCAP_TSTAMP_ADAPTER));
        Logging::log(warning, "Cannot set specified timestamp type (" + ts_type + ")");
    }
    set_direction = distinguish_capture_direction();
    Logging::log(debug, "Traffic will capture: " + Configuration::enum_to_str(static_cast<sniff_direction>(set_direction)));
    return 0;
}

int Collector::distinguish_capture_direction() {
    string filter_in, filter_out;

    switch (sensor_config.direction) {
        case promisc:
            if (pcap_set_promisc(capture_handle, 1) != 0) {
                throw CaptureError("Promiscuous mode attempt on a activated handle");
            }
            return promisc;
        case in:
            return set_capture_direction(PCAP_D_IN, in);
        case out:
            return set_capture_direction(PCAP_D_OUT, out);
        default:
            throw CaptureError("Unknown capture direction");
    }
}

sniff_direction Collector::set_capture_direction(pcap_direction_t pcap_dir, sniff_direction sensor_dir) {
    char errbuff[PCAP_ERRBUF_SIZE];
    bpf_u_int32 addr, mask;
    struct bpf_program filter_program;
    string filter;

    if (pcap_setdirection(capture_handle, pcap_dir) != PCAP_ERROR) {
        return sensor_dir;
    }

    Logging::log(warning, string(pcap_geterr(capture_handle)));

    filter = (device_ip_addr.empty()) ? FILTER_MAC(device_mac_addr, sensor_dir) : FILTER_IP(device_ip_addr, sensor_dir);
    pcap_lookupnet(sensor_config.interface.c_str(), &addr, &mask, errbuff);

    activate_handle(); // filter needs to be activated on an active capture handle !!

    if (pcap_compile(capture_handle, &filter_program, string(filter).c_str(), 0, mask) == PCAP_ERROR)
        throw CaptureError("Unable to compile provided filter: " + filter);
    if (pcap_setfilter(capture_handle, &filter_program) == PCAP_ERROR)
        throw CaptureError("Unable to set compiled filter");
    Logging::log(debug, "Capture direction set using filter '" + filter + "'");

    return sensor_dir;
}

int Collector::activate_handle() {
    int ret;

    ret = pcap_activate(capture_handle);

    if (ret > 0) Logging::log(warning, string(pcap_geterr(capture_handle)));
    else if (ret < 0 && ret != PCAP_ERROR_ACTIVATED) throw CaptureError(string(pcap_geterr(capture_handle)));
    else if (ret == PCAP_ERROR_ACTIVATED) Logging::log(warning, "Attempting to activate already active handle");
    else return 0;

    return 1;
}

int Collector::capture_network_packets(int packet_count) {
    if (capture_handle)
        pcap_loop(capture_handle, packet_count, packet_callback, reinterpret_cast<u_char*>(this));
}

void Collector::packet_callback(u_char *object, const struct pcap_pkthdr *meta, const u_char *bytes) {
    auto this_object = reinterpret_cast<Collector *>(object);
    int datalink = pcap_datalink(this_object->capture_handle);

    if (datalink == DLT_EN10MB) {
        Parser::process_packet(meta->len, meta->caplen, meta->ts, bytes, datalink);
    } else {
        Logging::log(warning, "received other linklayer than Ethernet");
    }
}

const string &Collector::getDevice_ip_addr() const {
    return device_ip_addr;
}

const string &Collector::getDevice_mac_addr() const {
    return device_mac_addr;
}

Collector::~Collector() {
    pcap_close(capture_handle);
}
