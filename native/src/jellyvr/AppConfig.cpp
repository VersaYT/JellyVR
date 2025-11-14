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

using json = nlohmann::json;
namespace fs = std::filesystem;

AppConfig::AppConfig() {
    settings.instantiate();
    initPaths();
    initConfigFile();
}

void AppConfig::initPaths() {
    String user_dir = ProjectSettings::get_singleton()->globalize_path("user://");

    this->ConfigPath = user_dir + "Config/";
    this->CachePath = user_dir + "Cache/";
    this->LogsPath = user_dir + "Logs/";

    this->ConfigFilePath = this->ConfigPath + "JellyVR.json";

    fs::create_directories(this->ConfigPath.utf8().get_data());
    fs::create_directories(this->CachePath.utf8().get_data());
    fs::create_directories(this->LogsPath.utf8().get_data());
}



bool AppConfig::initConfigFile() {
    fs::path config_file_path = this->ConfigFilePath.utf8().get_data();
    UtilityFunctions::print(this->ConfigFilePath);
    json config;

    std::ifstream file(config_file_path);

    // generate a new id if config file doesnt exist
    if(!file) {
        std::ifstream default_file("config/default_config.json");

        if (!default_file) {
            std::cerr << "Failed to open default config\n";
            UtilityFunctions::printerr("Failed to open default config");
            return false;
        }
        default_file >> config;

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
    ClassDB::bind_method(D_METHOD("get_username"), &AppConfig::get_username);
    ClassDB::bind_method(D_METHOD("set_settings", "settings"), &AppConfig::set_settings);
    ClassDB::bind_method(D_METHOD("get_settings"), &AppConfig::get_settings);
    ClassDB::bind_method(D_METHOD("set_collection_folders", "collection"), &AppConfig::set_collection_folders);
    ClassDB::bind_method(D_METHOD("set_config_value", "keys", "value"), &AppConfig::set_config_value);
    ClassDB::bind_method(D_METHOD("get_collection_folders"), &AppConfig::get_collection_folders);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "settings"), "set_settings", "get_settings");
}