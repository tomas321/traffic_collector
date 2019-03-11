//
// Created by tomas on 10/03/19.
//

#include <collector.h>

#include "collector.h"
#include "logging.h"
#include "exceptions.h"
#include "functions.h"
#include "configuration.h"

Collector::Collector(const sensor_settings &sensor_config, const filter_settings &filter_config) : sensor_config(
        sensor_config), filter_config(filter_config) {
    setup_handle();
}

void Collector::setup_handle() {
    Logging::log(info, "setting up capture handle on " + sensor_config.interface);
    set_capture_handle(sensor_config.interface);
    set_preferences();
}

int Collector::set_capture_handle(const string &device) {
    pcap_if_t *alldevs;
    char errbuff[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuff) == PCAP_ERROR) {
        Logging::log(fatal, "Couldn't find any available device to capture on");
        throw CaptureError("No available device");
    }
    pcap_if_t *current = alldevs;
    while (current != NULL) {
        if (string(current->name) == device) {
            capture_handle = pcap_create(current->name, errbuff);
            pcap_freealldevs(alldevs);
            return 0;
        }
        current = current->next;
    }
    throw CaptureError(device + " is not available capture device");
}

int Collector::set_preferences() {
    Logging::log(info, "Setting preferences for capture handle");

    int direction = set_direction_capture();
    Logging::log(debug, "Traffic will capture: " + Configuration::enum_to_str(static_cast<sniff_direction>(direction)));

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

//    only callable for activated handle
//    int *datalink_formats, n_formats;
//    n_formats = pcap_list_datalinks(capture_handle, &datalink_formats);
    return 0;

}

int Collector::set_direction_capture() {
    char errbuff[PCAP_ERRBUF_SIZE];
    bpf_u_int32 addr, mask;
    struct bpf_program filter_program;

    pcap_lookupnet(sensor_config.interface.c_str(), &addr, &mask, errbuff);
    switch (sensor_config.direction) {
        case promisc:
            pcap_set_promisc(capture_handle, 1);
            return promisc;
        case in:
            pcap_compile(capture_handle, &filter_program, string(FILTER_TO_INCOMING(convert_ip(addr))).c_str(), 0, mask);
            pcap_setfilter(capture_handle, &filter_program);
            return in;
        case out:
            pcap_compile(capture_handle, &filter_program, string(FILTER_TO_OUTGOING(convert_ip(addr))).c_str(), 0, mask);
            pcap_setfilter(capture_handle, &filter_program);
            return out;
        default:
            throw CaptureError("Unknown capture direction", error);
    }
}