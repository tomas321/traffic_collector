#ifndef TRAFFIC_COLLECTOR_COLLECTOR_H
#define TRAFFIC_COLLECTOR_COLLECTOR_H

#include <pcap/pcap.h>
#include <arpa/inet.h>

#include "configuration.h"
#include "processor.h"

#define GIGABIT_SEC_SIZE 125000000 // data in second on 1Gb/s
#define SNAPLEN 300
#define FILTER_IP(ip, dir) (dir == in) ? "dst host " + ip : "src host " + ip
#define FILTER_MAC(mac, dir) (dir == in) ? "ether dst " + mac : "ether src " + mac


class Collector {
private:
    sensor_settings sensor_config;
    pcap_t *capture_handle;
    string device_ip_addr;
    string device_mac_addr;
    uint8_t *raw_device_mac_addr;
    Processor *parser;

    /**
     * Main Collector setup function.
     */
    void setup_handle();

    /**
     * Searches for provided network device and initializes the capture handle.
     *
     * @param device The network device string.
     * @return 0 on success.
     */
    int set_capture_handle(const string &device);

    /**
     * Set all preferences fo the capture handle.
     *
     * @return 0 on success.
     */
    int set_preferences();

    /**
     * Set available timestamp type for capture handle.
     *
     * @param ts_type Desired type. If not supported its set to chosen type.
     * @return 0 on success.
     */
    int choose_capture_timestamping(int *ts_type);

    /**
     * Switch provided capture direction and call proper setters.
     *
     * @return Enumerated capture direction. on of [in, out, promisc].
     */
    int distinguish_capture_direction();

    /**
     * Set capture direction with native API otherwise using filters.
     *
     * @param pcap_dir Pcap enumerated capture direction.
     * @param sensor_dir Project specific enumerated capture direction.
     * @return Enumerated capture direction. on of [in, out, promisc].
     */
    sniff_direction set_capture_direction(pcap_direction_t pcap_dir, sniff_direction sensor_dir);

    /**
     * Activate capture handle if not activated before.
     *
     * @return 0 on success and 1 if warning is returned.
     */
    int activate_handle();

    /**
     * Main capture callback based on predefined typedef.
     *
     * @param object User defined parameter. In this case the Collector object.
     * @param meta Packet meta data.
     * @param bytes Packet bytes.
     */
    static void packet_callback(u_char *object, const struct pcap_pkthdr *meta, const u_char *bytes);

public:
    /**
     * Object constructor.
     *
     * @param sensor_config Sensor configuration structure.
     * @param parser Parser used for packet parsing
     */
    Collector(sensor_settings sensor_config, Processor *parser);

    /**
     * Object destructor closing the capture handle.
     */
    virtual ~Collector();

    /**
     * Indefinitely or definitely capture network packets.
     *
     * @param packet_count Number of packets to capture. On default the capture is infinite.
     * @return 0 on success.
     */
    int capture_network_packets(int packet_count = -1);

    /**
     * Get IP address of capture device.
     *
     * @return IP address string.
     */
    const string &getDevice_ip_addr() const;

    /**
     * Get MAC address of capture device.
     *
     * @return MAC address string.
     */
    const string &getDevice_mac_addr() const;

    /**
     * Get MAC address of capture device.
     *
     * @return Raw MAC address octets.
     */
    const uint8_t *get_raw_device_mac_addr() const;
};


#endif //TRAFFIC_COLLECTOR_COLLECTOR_H
