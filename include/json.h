//
// Created by tomas on 18/03/19.
//

#ifndef TRAFFIC_COLLECTOR_JSON_H
#define TRAFFIC_COLLECTOR_JSON_H

#include <string>
#include <iostream>
#include <map>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define INCOMPLETE "incomplete"

using namespace std;
using namespace rapidjson;

class Json {
private:
    /**
     * Json object string structure.
     */
    StringBuffer dictionary;

    /**
     * Dictionary writer.
     */
    Writer<StringBuffer> *json_writer;

    /**
     * Add integer value.
     *
     * @param key String key.
     * @param value Integer value.
     * @return 0 on success.
     */
    int add_specific(const string &key, int value);
    int add_specific(const string &key, uint32_t value);
    int add_specific(const string &key, uint16_t value);
    int add_specific(const string &key, uint8_t value);

    /**
     * Add string value.
     *
     * @param key String key.
     * @param value String value.
     * @return 0 on success.
     */
    int add_specific(const string &key, string value);
    int add_specific(const string &key, char *value);

    /**
     * Add double value.
     *
     * @param key String key.
     * @param value Double value.
     * @return 0 on success.
     */
    int add_specific(const string &key, double value);

    /**
     * Add boolean value.
     *
     * @param key String key.
     * @param value Boolean value.
     * @return 0 on success.
     */
    int add_specific(const string &key, bool value);

public:
    /**
     * Initialize Writer object with StringBuffer dictionary.
     */
    Json();

    /**
     * Add key-value pair to dictionary.
     *
     * @tparam T Typename of value.
     * @param key String key.
     * @param value Pair value.
     * @return 0 on success.
     */
    template<typename T>
    int add(string key, T value) {
        if (Json::add_specific(key, value)) return 0;
        else return 1;
    }

    void start_object(const string &key) {
        json_writer->Key(key.c_str());
        json_writer->StartObject();
    }

    void end_object() {
        json_writer->EndObject();
    }

    /**
     * Generate string from StringBuffer dictionary.
     *
     * @return String representation of dictionary (json string) if json is complete, else returns INCOMPLETE.
     */
    const string stringify();
};


#endif //TRAFFIC_COLLECTOR_JSON_H
