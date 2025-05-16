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
    ClassDB::bind_method(D_METHOD("login", "Username", "Pw"), &Auth::login);
}

void Auth::set_server_provider(IServerProvider* provider) {
    server_provider = provider;
}

bool Auth::login(String Username, String Pw) {
    CURL *curl;
    CURLcode result;
    std::string response;
    std::string endpoint_str = "/Users/AuthenticateByName";
    std::string server_url = server_provider->get_server_url();

    std::string c_username = Username.utf8().get_data();
    std::string c_pw = Pw.utf8().get_data();

    std::string json_body = R"({"Username":")" + c_username + R"("Pw":")" + c_pw + R"("})";

    curl = curl_easy_init();
    if(curl == NULL) {
        UtilityFunctions::print("HTTP request failed");
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, server_url + endpoint_str);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Authorization: ");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    result = curl_easy_perform(curl);

    try {
        if (!response.empty()) {
            json j = json::parse(response);
            std::string server_name = j["ServerName"];
            UtilityFunctions::print(String::utf8(server_name.c_str()));
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
    return true;
}

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