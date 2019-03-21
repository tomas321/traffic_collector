//
// Created by tomas on 18/03/19.
//

#include <json.h>

Json::Json() {
    json_writer = new Writer<StringBuffer>(dictionary);
    json_writer->StartObject();
}

int Json::add_specific(const string &key, int value) {
    return (json_writer->Key(key.c_str()) && json_writer->Int(value));
}

int Json::add_specific(const string &key, uint32_t value) {
    return (json_writer->Key(key.c_str()) && json_writer->Uint(value));
}

int Json::add_specific(const string &key, uint16_t value) {
    return (json_writer->Key(key.c_str()) && json_writer->Uint(value));
}

int Json::add_specific(const string &key, uint8_t value) {
    return (json_writer->Key(key.c_str()) && json_writer->Uint(value));
}

int Json::add_specific(const string &key, bool value) {
    return (json_writer->Key(key.c_str()) && json_writer->Bool(value));
}

int Json::add_specific(const string &key, double value) {
    return (json_writer->Key(key.c_str()) && json_writer->Double(value));
}

int Json::add_specific(const string &key, string value) {
    return (json_writer->Key(key.c_str()) && json_writer->String(value.c_str()));
}

int Json::add_specific(const string &key, char *value) {
    return (json_writer->Key(key.c_str()) && json_writer->String(value));
}

const string Json::stringify() {
    json_writer->EndObject();
    return json_writer->IsComplete() ? dictionary.GetString() : INCOMPLETE;
}