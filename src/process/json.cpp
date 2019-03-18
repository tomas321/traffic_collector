//
// Created by tomas on 18/03/19.
//

#include <json.h>

Json::Json() {
    json_writer = new Writer<StringBuffer>(dictionary);
    json_writer->StartObject();
}

int Json::add_specific(const string &key, int value) {
    json_writer->Key(key.c_str());
    json_writer->Int(value);

    return 0;
}

int Json::add_specific(const string &key, bool value) {
    json_writer->Key(key.c_str());
    json_writer->Bool(value);

    return 0;
}

int Json::add_specific(const string &key, double value) {
    json_writer->Key(key.c_str());
    json_writer->Double(value);

    return 0;
}

int Json::add_specific(const string &key, string value) {
    json_writer->Key(key.c_str());
    json_writer->String(value.c_str());

    return 0;
}

const string Json::stringify() {
    json_writer->EndObject();
    return json_writer->IsComplete() ? dictionary.GetString() : INCOMPLETE;
}