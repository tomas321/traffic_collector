//
// Created by tomas on 10/03/19.
//

#ifndef TRAFFIC_COLLECTOR_FUNCTIONS_H
#define TRAFFIC_COLLECTOR_FUNCTIONS_H

const string convert_ip(uint32_t number_ip) {
    char ip_address[16];
    snprintf(ip_address,sizeof ip_address,"%u.%u.%u.%u" ,(number_ip & 0xff000000) >> 24
            ,(number_ip & 0x00ff0000) >> 16
            ,(number_ip & 0x0000ff00) >> 8
            ,(number_ip & 0x000000ff));
    // depending on the byte order your GetHostIp() returns the IP address in
    // you might need to reverse the above (i.e. print (number_ip &0x000000ff)) first.
    string str_ip = string(ip_address);
    std::cout << "control print: " << str_ip << endl;
    return str_ip;
}

#endif //TRAFFIC_COLLECTOR_FUNCTIONS_H
