#include "Auth.h"
#include <curl/curl.h>
#include <stdio.h>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Auth::Auth() {
    fprintf(stdout, "Auth constructor called");
}

Auth::~Auth() {
    fprintf(stdout, "Freeing Auth");
}

void Auth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_ping", "url"), &Auth::_ping);
}

bool Auth::_ping(const String& url) {
    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();
    if(curl == NULL) {
        UtilityFunctions::print("HTTP request failed");
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.utf8().get_data());

    result = curl_easy_perform(curl);

    if (result != CURLE_OK) {
        UtilityFunctions::print(curl_easy_strerror(result));
        return -1;
    }
    
    long response_code = 0;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    UtilityFunctions::print("HTTP Response Code: ", response_code);

    curl_easy_cleanup(curl);
    return true;
}