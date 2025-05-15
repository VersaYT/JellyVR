#include "Auth.h"
#include <curl/curl.h>
#include <stdio.h>
#include "utils/Network.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <nlohmann/json.hpp>
#include <regex>

using namespace godot;
using json = nlohmann::json;

Auth::Auth() {
    fprintf(stdout, "Auth constructor called");
}

Auth::~Auth() {
    fprintf(stdout, "Freeing Auth");
}

void Auth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("ping", "url"), &Auth::ping);
}

// bool Auth::login(const String& name, const String& passw) {
//     CURL *curl;
//     CURLcode result;

//     curl = curl_easy_init();
// }

String Auth::ping(String url) {
    CURL *curl;
    CURLcode result;
    String endpoint_str = "/System/Info/Public";
    std::string response;
    std::regex pattern("jellyfin", std::regex_constants::icase);


    curl = curl_easy_init();
    if(curl == NULL) {
        UtilityFunctions::print("HTTP request failed");
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, (url + endpoint_str).utf8().get_data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    result = curl_easy_perform(curl);

    if (result != CURLE_OK) {
        UtilityFunctions::print(curl_easy_strerror(result));
        curl_easy_cleanup(curl);
        return "";
    }

    try {
        if (!response.empty()) {
            json j = json::parse(response);
            if (j.contains("ProductName")) {
                std::string productName = j["ProductName"];
                    if(std::regex_search(productName, pattern)) {
                        UtilityFunctions::print("Jellyfin server found");
                        return url.utf8().get_data();
                    } else {
                        UtilityFunctions::print("The url is not a valid Jellyfin server");
                    }
            } else {
                UtilityFunctions::print("Response does not contain 'productName' field");
            }
        } else {
            UtilityFunctions::print("Empty response received");
        }
    } catch (const json::parse_error& e) {
        UtilityFunctions::print("JSON parse error: ", String(e.what()));
        curl_easy_cleanup(curl);
        return "";
    } catch (const std::exception& e) {
        UtilityFunctions::print("Exception in JSON handling: ", String(e.what()));
        curl_easy_cleanup(curl);
        return "";
    }

    curl_easy_cleanup(curl);
    return "";
}