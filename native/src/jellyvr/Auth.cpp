#include "Auth.h"
#include <curl/curl.h>
#include <stdio.h>
#include "utils/Network.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <nlohmann/json.hpp>
#include <regex>
#include "include/config.h"
#include "include/version.h"
#include "utils/ConfigPath.h"
#include <fstream>

using namespace godot;
using json = nlohmann::json;
namespace fs = std::filesystem;

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

bool Auth::login(String Username, String Pw, Ref<AppConfig> config) {
    CURL *curl;
    CURLcode result;
    long http_code = 0;
    std::regex protocol_regex(R"(https?://)");
    std::string DeviceId = config->DeviceId.utf8().get_data();
    std::string response;
    std::string endpoint_str = "/Users/AuthenticateByName";
    std::string server_url = server_provider->get_server_url();
    std::string host_name = std::regex_replace(server_url, std::regex(R"(https?://)"), "");

    std::string c_full_url = server_url + endpoint_str;
    String full_url = String::utf8(c_full_url.c_str());

    std::string c_username = Username.utf8().get_data();
    std::string c_pw = Pw.utf8().get_data();

    std::string json_body = R"({"Username":")" + c_username + R"(", "Pw":")" + c_pw + R"("})";

    curl = curl_easy_init();
    if(curl == NULL) {
        UtilityFunctions::print("HTTP request failed");
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, c_full_url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    std::string authorization_header = ("Authorization: MediaBrowser ");
    authorization_header.append("Client=\"").append(CLIENT_NAME).append("\", ");
    authorization_header.append("Version=\"").append(PROJECT_VERSION).append("\", ");
    authorization_header.append("DeviceId=\"").append(DeviceId).append("\", ");
    authorization_header.append("Device=\"").append(DEVICE_NAME).append("\" ");

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, authorization_header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
        UtilityFunctions::print("cURL perform failed: ", curl_easy_strerror(result));
        return false;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    UtilityFunctions::print("HTTP response code: ", http_code);

    try {
        if (!response.empty()) {
            json parsed_response = json::parse(response);

            fs::path config_file_path = getConfigPath("JellyVR");

            std::ifstream file_in(config_file_path);
            if (!file_in.is_open()) {
                std::cerr << "Failed to open config.json\n";
                return 1;
            }
            json j;
            file_in >> j;
            file_in.close();

            std::string access_token = parsed_response["AccessToken"];
            std::string user_id = parsed_response["User"]["Id"];
            std::string user_name = parsed_response["User"]["Name"];
            std::string server_id = parsed_response["User"]["ServerId"];

            j["User"]["AccessToken"] = access_token;
            j["User"]["UserId"] = user_id;
            j["User"]["UserName"] = user_name;
            j["User"]["ServerId"] = server_id;

            std::ofstream file_out(config_file_path);
            if (!file_out.is_open()) {
                std::cerr << "Failed to open config.json for writing\n";
                return 1;
            }
            file_out << j.dump(4);
            file_out.close();

            config->AccessToken = String::utf8(access_token.c_str());
            config->UserId = String::utf8(user_id.c_str());
            config->UserName = String::utf8(user_name.c_str());
            config->ServerId = String::utf8(server_id.c_str());

            UtilityFunctions::print(config->UserName);
        } else {
            UtilityFunctions::print("Empty response received");
            return false;
        }
    } catch (const json::parse_error& e) {
        UtilityFunctions::print("JSON parse error: ", String(e.what()));
        curl_easy_cleanup(curl);
        return false;
    } catch (const std::exception& e) {
        UtilityFunctions::print("Exception in JSON handling: ", String(e.what()));
        curl_easy_cleanup(curl);
        return false;
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