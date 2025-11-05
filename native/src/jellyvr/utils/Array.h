#ifndef ARRAY_H
#define ARRAY_H

#include <nlohmann/json.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Variant json_to_variant(const nlohmann::json& value) {
    if (value.is_string()) {
        return String(value.get<std::string>().c_str());
    } else if (value.is_boolean()) {
        return Variant(bool(value));
    } else if (value.is_number_integer()) {
        return Variant(int(value));
    } else if (value.is_number_float()) {
        return Variant(double(value));
    } else if (value.is_array()) {
        Array arr;
        for (const auto& el : value) {
            arr.append(json_to_variant(el));
        }
        return arr;
    } else if (value.is_object()) {
        Dictionary dict;
        for (auto it = value.begin(); it != value.end(); ++it) {
            String key = String(it.key().c_str());
            dict[key] = json_to_variant(it.value());
        }
        return dict;
    }

    // Null or unsupported
    return Variant(); // Null variant
}

// Main function to convert top-level JSON array to Godot Array
void json_to_godot_array(const nlohmann::json& json_data, Array& output_array) {
    if (!json_data.is_array()) {
        // Optionally throw or log
        return;
    }

    output_array.clear();

    for (const auto& item : json_data) {
        output_array.append(json_to_variant(item));
    }
}

#endif