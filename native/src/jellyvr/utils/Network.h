#ifndef NETWORK_H
#define NETWORK_H
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

#include <cstddef>

using namespace godot;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

Dictionary parse_headers_to_dict(const PackedStringArray &headers);

#endif