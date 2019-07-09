#include "collector.h"
#include "logging.h"
#include "exceptions.h"
#include "processor.h"

#include <netinet/in.h>
#include <linux/sockios.h>
#include <libnet.h>

#ifdef __linux
#define LINK_PF PF_PACKET
#elif __unix
#define LINK_PF AF_LINK
#endif

Collector::Collector(sensor_settings sensor_config, Processor *parser) : sensor_config(std::move(sensor_config)),
                                                                         parser(parser) {
    device_ip_addr = "";
    device_mac_addr = "";
    setup_handle();
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
    raw_device_mac_addr = (uint8_t *) malloc(6);
    char mac_addr[18];
    struct ifreq ifr;
    int s;

    if (pcap_findalldevs(&alldevs, errbuff) == PCAP_ERROR) {
        throw CaptureError("Couldn't find any available device to capture on", critical);
    }

    for (pcap_if_t *current = alldevs; current != NULL; current = current->next) {
        if (string(current->name) == device) {
            Logging::log(debug, "Found " + device);
            for (pcap_addr_t *address = current->addresses; address != NULL; address = address->next) {
                switch (address->addr->sa_family) {
                    case LINK_PF:
                        s = socket(AF_INET, SOCK_DGRAM, 0);
                        strcpy(ifr.ifr_name, "wlo1");
                        ioctl(s, SIOCGIFHWADDR, &ifr);
                        close(s);
                        strcpy(reinterpret_cast<char *>(raw_device_mac_addr),
                               reinterpret_cast<const char *>((unsigned char *) ifr.ifr_hwaddr.sa_data));
                        snprintf(mac_addr, sizeof(mac_addr), "%02x:%02x:%02x:%02x:%02x:%02x", ((unsigned char *) ifr.ifr_hwaddr.sa_data)[0],
                                 ((unsigned char *) ifr.ifr_hwaddr.sa_data)[1], ((unsigned char *) ifr.ifr_hwaddr.sa_data)[2],
                                 ((unsigned char *) ifr.ifr_hwaddr.sa_data)[3], ((unsigned char *) ifr.ifr_hwaddr.sa_data)[4],
                                 ((unsigned char *) ifr.ifr_hwaddr.sa_data)[5]);

//                        char mac_addr[18];
//                        strcpy(reinterpret_cast<char *>(raw_device_mac_addr),
//                               reinterpret_cast<const char *>((struct sockaddr_ll *) address->addr->sa_data));
//                        snprintf(mac_addr, sizeof(mac_addr), "%02x:%02x:%02x:%02x:%02x:%02x", raw_device_mac_addr[0],
//                                 raw_device_mac_addr[1], raw_device_mac_addr[2],
//                                 raw_device_mac_addr[3], raw_device_mac_addr[4], raw_device_mac_addr[5]);
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
    int ts_type = PCAP_TSTAMP_ADAPTER;
    Logging::log(info, "Setting preferences for capture handle");

    if (pcap_set_snaplen(capture_handle, SNAPLEN) != 0)
        Logging::log(warning, "Cannot set specified snaplen (" + to_string(SNAPLEN) + ")");
    if (pcap_set_immediate_mode(capture_handle, 1) != 0)
        Logging::log(warning, "Cannot set immediate mode");
    if (pcap_set_buffer_size(capture_handle, GIGABIT_SEC_SIZE) != 0)
        Logging::log(warning, "Cannot set specified buffer size (" + to_string(GIGABIT_SEC_SIZE) + ")");
    if (choose_capture_timestamping(&ts_type) != 0) {
        Logging::log(warning,
                     "Cannot set specified timestamp type (" + string(pcap_tstamp_type_val_to_name(ts_type)) + ")");
    }
    set_direction = distinguish_capture_direction();
    Logging::log(info,
                 "Traffic will capture: " + Configuration::enum_to_str(static_cast<sniff_direction>(set_direction)));
    return 0;
}

int Collector::choose_capture_timestamping(int *ts_type) {
    int *types;
    int count = 0;

    if (!capture_handle) {
        throw CaptureError("Must create pcap handle first");
    }

    if (pcap_set_tstamp_type(capture_handle, *ts_type) == 0) {
        Logging::log(debug,
                     "Pcap capture timestamp set to desired type (" + string(pcap_tstamp_type_val_to_name(*ts_type)) +
                     ")");
        return 0;
    }
    count = pcap_list_tstamp_types(capture_handle, &types);
    if (count == PCAP_ERROR || count == 0) {
        Logging::log(warning, "Cannot retrieve timestamp types");
        pcap_free_tstamp_types(types);
        return 1;
    } else {
        pcap_set_tstamp_type(capture_handle, types[0]);
        *ts_type = types[0];
        Logging::log(debug, "Pcap capture timestamp set to (" + string(pcap_tstamp_type_val_to_name(*ts_type)) + ")");
        pcap_free_tstamp_types(types);
        return 0;
    }
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

//    if (pcap_setdirection(capture_handle, pcap_dir) != PCAP_ERROR) {
//        return sensor_dir;
//    }
//
//    Logging::log(warning, string(pcap_geterr(capture_handle)));

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
    else if (ret == PCAP_ERROR_ACTIVATED) Logging::log(warning, "Failed attempt to activate already active handle");
    else return 0;

    return 1;
}

int Collector::capture_network_packets(int packet_count) {
    Logging::log(info, "Starting packet capture");
    if (capture_handle)
        pcap_loop(capture_handle, packet_count, packet_callback, reinterpret_cast<u_char *>(this));
    Logging::log(info, "Finished. captured " + to_string(packet_count));
}

void Collector::packet_callback(u_char *object, const struct pcap_pkthdr *meta, const u_char *bytes) {
    auto this_object = reinterpret_cast<Collector *>(object);
    int datalink = pcap_datalink(this_object->capture_handle);

    if (datalink == DLT_EN10MB) {
        this_object->parser->process_packet(meta->len, meta->caplen, meta->ts, bytes, this_object->raw_device_mac_addr);
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

const uint8_t *Collector::get_raw_device_mac_addr() const {
    return raw_device_mac_addr;
}

Collector::~Collector() {
    pcap_close(capture_handle);
    free(raw_device_mac_addr);
}
