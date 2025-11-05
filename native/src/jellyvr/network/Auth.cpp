#include "Auth.h"
#include <curl/curl.h>
#include <stdio.h>
#include "../utils/Network.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <nlohmann/json.hpp>
#include <regex>
#include "../include/config.h"
#include "../AppConfig.h"
#include "../include/version.h"
#include "../utils/Array.h"
#include <fstream>
#include <godot_cpp/variant/variant.hpp>


using namespace godot;
using json = nlohmann::json;
namespace fs = std::filesystem;

Auth::Auth() 
: is_logged_in(false) {
    UtilityFunctions::print("Auth constructor called");
}

Auth::~Auth() {
    fprintf(stdout, "Freeing Auth");
}

void Auth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("ping", "url"), &Auth::ping);
    ClassDB::bind_method(D_METHOD("login", "Username", "Pw"), &Auth::login);
    ClassDB::bind_method(D_METHOD("logout"), &Auth::logout);
    ClassDB::bind_method(D_METHOD("get_public_users"), &Auth::get_public_users);
    ClassDB::bind_method(D_METHOD("get_user_profile_pic"), &Auth::get_user_profile_pic);
    ClassDB::bind_method(D_METHOD("get_is_logged_in"), &Auth::get_is_logged_in);
    ClassDB::bind_method(D_METHOD("_on_json_request_completed"), &Auth::_on_json_request_completed);
    ADD_SIGNAL(MethodInfo("error_occurred", PropertyInfo(Variant::STRING, "message")));
}

void Auth::_on_json_request_completed(Variant data, Ref<Json> json) {
    UtilityFunctions::print("Freeing json request");
    json_requests.erase(json);
}

bool Auth::get_is_logged_in() {
    return is_logged_in;
}

void Auth::set_is_logged_in(bool value) {
    is_logged_in = value;
}

bool Auth::login(String Username, String Pw, Ref<AppConfig> config, Ref<NetworkConfig> network_config) {

    CURL *curl;
    CURLcode result;
    long http_code = 0;
    std::regex protocol_regex(R"(https?://)");
    std::string DeviceId = config->get_device_id().utf8().get_data();
    std::string response;
    std::string endpoint_str = "/Users/AuthenticateByName";
    std::string server_url = network_config->get_server_url().utf8().get_data();
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

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, network_config->get_authorization_header().utf8().get_data());
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

            fs::path config_file_path = config->ConfigFilePath.utf8().get_data();

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

            config->set_access_token(String::utf8(access_token.c_str()));
            config->set_user_id(String::utf8(user_id.c_str()));
            config->set_username(String::utf8(user_name.c_str()));
            config->set_server_id(String::utf8(server_id.c_str()));

            set_is_logged_in(true);
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

bool Auth::logout(Ref<AppConfig> config) {
    fs::path config_file_path = config->ConfigFilePath.utf8().get_data();

    std::ifstream file_in(config_file_path);
    if (!file_in.is_open()) {
        std::cerr << "Failed to open config.json\n";
        return false;
    }
    json j;
    file_in >> j;
    file_in.close();

    j["User"]["AccessToken"] = "";

    std::ofstream file_out(config_file_path);
    if (!file_out.is_open()) {
        std::cerr << "Failed to open config.json for writing\n";
        return false;
    }
    file_out << j.dump(4);
    file_out.close();

    config->set_access_token("");
    set_is_logged_in(false);
    UtilityFunctions::print("Logout was successfull");

    return true;
}

Ref<Json> Auth::get_public_users(Node *node, Ref<NetworkConfig> network_config) {
    std::string server_url = network_config->get_server_url().utf8().get_data();
    UtilityFunctions::print(server_url.c_str());
    std::string endpoint_str = "/Users/Public";
    std::string c_full_url = "http://" + server_url + endpoint_str;
    String full_url = c_full_url.c_str();

    Ref<Json> json = memnew(Json(node, network_config));
    json_requests.push_back(json);
    json->connect("request_completed_signal", Callable(this, "_on_json_request_completed").bind(json));
    json->json_get_request(full_url);

    return json;
}

Ref<Json> Auth::get_user_profile_pic(Node *node, String user_id, Ref<NetworkConfig> network_config) {
    std::string server_url = network_config->get_server_url().utf8().get_data();
    UtilityFunctions::print(server_url.c_str());
    String endpoint_str = "/UserImage?userId=" + user_id + "&tag=true&format=Webp";
    std::string c_full_url = "http://" + server_url + endpoint_str.utf8().get_data();
    String full_url = c_full_url.c_str();

    Ref<Json> json = memnew(Json(node, network_config));
    json_requests.push_back(json);
    json->connect("request_completed_signal", Callable(this, "_on_json_request_completed").bind(json));
    json->json_get_request(full_url);

    return json;
}


bool Auth::ping(String url) {
    CURL *curl;
    CURLcode result;
    String endpoint_str = "/System/Info/Public";
    std::string response;
    std::regex pattern("jellyfin", std::regex_constants::icase);


    curl = curl_easy_init();
    if(curl == NULL) {
        UtilityFunctions::print("HTTP request failed");
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, (url + endpoint_str).utf8().get_data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    result = curl_easy_perform(curl);

    if (result != CURLE_OK) {
        UtilityFunctions::print(curl_easy_strerror(result));
        emit_signal("error_occurred", curl_easy_strerror(result));
        curl_easy_cleanup(curl);
        return false;
    }

    try {
        if (!response.empty()) {
            json j = json::parse(response);
            if (j.contains("ProductName")) {
                std::string productName = j["ProductName"];
                    if(std::regex_search(productName, pattern)) {
                        UtilityFunctions::print("Jellyfin server found");
                        return true;
                    } else {
                        UtilityFunctions::print("The url is not a valid Jellyfin server");
                        emit_signal("error_occurred", "The url is not a valid Jellyfin server");
                        return false;
                    }
            } else {
                UtilityFunctions::print("Response does not contain 'productName' field");
                emit_signal("error_occurred", "Response does not contain 'productName' field");
                return false;
            }
        } else {
            UtilityFunctions::print("Empty response received");
            emit_signal("error_occurred", "Empty response received");
            return false;
        }
    } catch (const json::parse_error& e) {
        UtilityFunctions::print("JSON parse error: ", String(e.what()));
        emit_signal("error_occurred", String(e.what()));
        curl_easy_cleanup(curl);
        return false;
    } catch (const std::exception& e) {
        UtilityFunctions::print("Exception in JSON handling: ", String(e.what()));
        emit_signal("error_occurred", String(e.what()));
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return false;
}