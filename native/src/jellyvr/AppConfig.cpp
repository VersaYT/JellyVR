#include "AppConfig.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include "third_party/stduuid/uuid.h"
#include <nlohmann/json.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/string.hpp>
#include "include/config.h"
#include"include/version.h"
#include <curl.h>
#include "utils/Network.h"
#include "utils/File.h"
#include "utils/DebugCurl.hpp"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "third_party/zip/zip.h"
#include <godot_cpp/classes/file_access.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

AppConfig::AppConfig() {
    settings.instantiate();
}

void AppConfig::initPaths() {
    String user_dir = ProjectSettings::get_singleton()->globalize_path("user://");

    this->ConfigPath = user_dir + "Config/";
    this->CachePath = user_dir + "Cache/";
    this->BinPath = user_dir + "Bin/";
    this->TmpPath = user_dir + "Tmp/";
    this->SSLPath = user_dir + "SSL/";

    this->ConfigFilePath = this->ConfigPath + "JellyVR.json";

    fs::create_directories(this->ConfigPath.utf8().get_data());
    fs::create_directories(this->CachePath.utf8().get_data());
    fs::create_directories(this->BinPath.utf8().get_data());
    fs::create_directories(this->TmpPath.utf8().get_data());
    fs::create_directories(this->SSLPath.utf8().get_data());
}

void AppConfig::init() {
    initPaths();
    initConfigFile();
    initSSL();
    #if defined(_WIN32) || defined(__linux__)
    // for now no yt-dlp on android, need a way to get a runtime binary for android for it
        fetch_yt_dlp();
    #endif
}


ReturnedData AppConfig::fetch_github_direct_download_link(std::string repo_path, std::string target_platform) {
    CURL *curl;
    CURLcode result;
    long http_code = 0;
    ReturnedData data;
    String cacert_file_path = this->SSLPath + "cacert.pem";
    std::string cafile = cacert_file_path.utf8().get_data();
    
    std::string response;
    std::string full_url = "https://api.github.com/repos/" + repo_path + "/releases/latest";

    data.download_link = "";
    data.http_operation_success = false;
    data.is_file_zip = false;

    curl = curl_easy_init();
    if(curl == NULL) {
        UtilityFunctions::print("HTTP request failed");
        return data;
    }

    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    // curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_curl_debug_callback);
    // curl_easy_setopt(curl, CURLOPT_DEBUGDATA, nullptr);
    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "/data/data/com.versa_studio.jellyvr/files/SSL/cacert.pem");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    string user_agent = "JellyVR/";
    user_agent += PROJECT_VERSION;
    curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK) {
        UtilityFunctions::print(curl_easy_strerror(result));
        emit_signal("error_occurred", curl_easy_strerror(result));
        return data;
    }

    json parsed_str = json::parse(response);
    json packages_array = parsed_str["assets"];

    for(auto& element : packages_array.items()) {
        auto &obj = element.value();
        std::string item_name_val = obj["name"];        
        if(obj["name"] == target_platform) {
            data.download_link = obj["browser_download_url"];
            data.http_operation_success = true;
            obj["content_type"] == "application/zip" ? data.is_file_zip = true : data.is_file_zip = false;
            return data;
        }
    }
    return data;
}

int on_extract_entry(const char *filename, void *arg) {
    static int i = 0;
    int n = *(int *)arg;
    printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

    return 0;
}

bool AppConfig::fetch_yt_dlp() {
    fs::path config_file_path = this->ConfigFilePath.utf8().get_data();
    json config;
    std::ifstream file(config_file_path);

    file >> config;
    std::string yt_dlp_release_date = config["YtDlp"]["ReleaseDate"];

    std::string target;
    String cacert_file_path = this->SSLPath + "cacert.pem";
    std::string cafile = cacert_file_path.utf8().get_data();

    // UtilityFunctions::print(cacert_file_path);

    std::string output_path = this->BinPath.utf8().get_data();
    std::string tmp_file_path = this->TmpPath.utf8().get_data();
    output_path += "yt-dlp/";
    ReturnedData data;

    #if defined(_WIN32)
    target = "yt-dlp.exe";
    std::string binary_name = "yt-dlp.exe";
    data = fetch_github_direct_download_link("yt-dlp/yt-dlp", target);
    tmp_file_path += target;
    #elif defined(__ANDROID__)
    target = "yt-dlp_linux_aarch64.zip";
    std::string binary_name = "yt-dlp_linux_aarch64";
    data = fetch_github_direct_download_link("yt-dlp/yt-dlp", target);
    tmp_file_path += target;
    #elif defined(__linux__)
    target = "yt-dlp_linux.zip";
    std::string binary_name = "yt-dlp_linux";
    data = fetch_github_direct_download_link("yt-dlp/yt-dlp", target);
    tmp_file_path += target;
    #endif

    if(data.http_operation_success == false) {
        UtilityFunctions::print("error fetching yt-dlp direct link from github");
    } else {
        string sliced_download_link = data.download_link.substr(51,10);
        UtilityFunctions::print(sliced_download_link.c_str());
        if(yt_dlp_release_date != sliced_download_link) {
            UtilityFunctions::print("Downloading new version of yt-dlp");
            CURL *curl = curl_easy_init();
            if (!curl) return false;
            UtilityFunctions::print(tmp_file_path.c_str());

            std::ofstream out(tmp_file_path.c_str(), std::ios::binary);
            if (!out.is_open()) return false;

            UtilityFunctions::print(data.download_link.c_str());
            // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            // curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_curl_debug_callback);
            // curl_easy_setopt(curl, CURLOPT_DEBUGDATA, nullptr);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_CAINFO, "/data/data/com.versa_studio.jellyvr/files/SSL/cacert.pem");
            curl_easy_setopt(curl, CURLOPT_URL, data.download_link.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFileCallback);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
            
            string user_agent = "JellyVR/";
            user_agent += PROJECT_VERSION;
            curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());


            CURLcode res = curl_easy_perform(curl);


            if (res == CURLE_OK) {
                UtilityFunctions::print("yt-dlp download was successfull");
            } else {
                UtilityFunctions::printerr(res);
            }
            out.close();
            curl_easy_cleanup(curl);
            UtilityFunctions::print(data.is_file_zip);
            if(data.is_file_zip) {

                int arg = 2;
                zip_extract(tmp_file_path.c_str(), output_path.c_str(), on_extract_entry, &arg);
            } else {
                fs::rename(tmp_file_path, output_path + target);
            }
            yt_dlp_binary_name = binary_name.c_str();
            // #if defined(__linux__) || defined(__ANDROID__)
            //     chmod(output_path.c_str(), 0755);
            // #endif

            PackedStringArray keys;
            keys.push_back("YtDlp");
            keys.push_back("ReleaseDate");
            set_config_value(keys, sliced_download_link.c_str());
            return res == CURLE_OK;
        }

    }
    return false;
}

void AppConfig::initSSL() {
    fs::path ssl_file_path = this->SSLPath.utf8().get_data();
    ssl_file_path /= "cacert.pem";

    std::ifstream ssl_file(ssl_file_path, std::ios::binary);
    if(!ssl_file) {
        ssl_file.close();

        String pem_file_content = FileAccess::get_file_as_string("res://SSL/cacert.pem");
        CharString cs = pem_file_content.utf8();

        std::ofstream new_file(ssl_file_path, std::ios::binary);
        new_file.write(cs.get_data(), cs.length());  // <--- write all bytes exactly
        new_file.close();
    }
}

bool AppConfig::initConfigFile() {
    fs::path config_file_path = this->ConfigFilePath.utf8().get_data();
    UtilityFunctions::print(this->ConfigFilePath);
    json config;

    std::ifstream file(config_file_path);

    // generate a new id if config file doesnt exist
    if(!file) {
        String default_config_json = FileAccess::get_file_as_string("res://config/default_config.json");
        config = json::parse(default_config_json.utf8().get_data());

        std::random_device rd;
        std::mt19937 gen(rd());
        uuids::basic_uuid_random_generator<std::mt19937> uuid_gen{gen};
        uuids::uuid id = uuid_gen();
        std::string id_str = uuids::to_string(id);

        config["User"]["DeviceId"] = id_str;
        DeviceId = String::utf8(id_str.c_str());

        std::ofstream new_file(config_file_path);
        new_file << config.dump(4);
        new_file.close();

        return true;
    } else {

    try {
        file >> config;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }

    std::cout << "Parsed config:\n" << config.dump(4) << std::endl;

    std::string c_DeviceId = config["User"]["DeviceId"];
    DeviceId = String::utf8(c_DeviceId.c_str());
    return true;
    }
}

void AppConfig::set_config_value(PackedStringArray keys, Variant value) {
    
    fs::path config_file_path = this->ConfigFilePath.utf8().get_data();
    json config;
    std::ifstream file(config_file_path);

    file >> config;
    json *current = &config;

    for(int i = 0; i < keys.size(); i++) {
        const std::string& key = keys[i].utf8().get_data();
        if(i == keys.size() -1) {
            switch (value.get_type())
            {
                case Variant::Type::STRING: {
                    godot::String gstr = value.operator godot::String();
                    (*current)[key] = gstr.utf8().get_data();
                    break;
                }
                case Variant::Type::INT: {
                    int g_int = value;
                    (*current)[key] = g_int;
                    break;
                }
            }   
        } else {
            current = &((*current)[key]);
        }
    }

    std::ofstream updated_file(config_file_path);

    updated_file << config.dump(4);

    updated_file.close();
}

AppConfig::~AppConfig() {
    fprintf(stdout, "freeing up AppConfig");
}

void AppConfig::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_app_version"), &AppConfig::get_app_version);
    ClassDB::bind_method(D_METHOD("get_access_token"), &AppConfig::get_access_token);
    ClassDB::bind_method(D_METHOD("get_device_id"), &AppConfig::get_device_id);
    ClassDB::bind_method(D_METHOD("get_server_id"), &AppConfig::get_server_id);
    ClassDB::bind_method(D_METHOD("get_user_id"), &AppConfig::get_user_id);
    ClassDB::bind_method(D_METHOD("init"), &AppConfig::init);
    ClassDB::bind_method(D_METHOD("get_username"), &AppConfig::get_username);
    ClassDB::bind_method(D_METHOD("set_settings", "settings"), &AppConfig::set_settings);
    ClassDB::bind_method(D_METHOD("get_settings"), &AppConfig::get_settings);
    ClassDB::bind_method(D_METHOD("set_yt_dlp_binary_name", "name"), &AppConfig::set_yt_dlp_binary_name);
    ClassDB::bind_method(D_METHOD("get_yt_dlp_binary_name"), &AppConfig::get_yt_dlp_binary_name);
    ClassDB::bind_method(D_METHOD("set_collection_folders", "collection"), &AppConfig::set_collection_folders);
    ClassDB::bind_method(D_METHOD("set_config_value", "keys", "value"), &AppConfig::set_config_value);
    ClassDB::bind_method(D_METHOD("get_collection_folders"), &AppConfig::get_collection_folders);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "settings"), "set_settings", "get_settings");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "yt_dlp_binary_name"), "set_yt_dlp_binary_name", "get_yt_dlp_binary_name");
}