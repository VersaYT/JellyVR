#include <cstddef>
#include <string>
#include <iostream>
#include "Network.h"
#include <vector>
#include <stdint.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);

    std::cout << "from write callback";
    return size * nmemb;
}

using namespace godot;

Dictionary parse_headers_to_dict(const PackedStringArray &headers) {
    Dictionary header_dict;
    UtilityFunctions::print("headers.size is" + headers.size());

    for (int i = 0; i < headers.size(); i++) {
        String line = headers[i];

        int separator_index = line.find(":");

        if (separator_index != -1) {
            String key = line.substr(0, separator_index).strip_edges();
            String value = line.substr(separator_index + 1).strip_edges();
            header_dict[key] = value;
        }
    }

    return header_dict;
}

