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

    std::ifstream file(config_file_path);

    // generate a new id if config file doesnt exist
    if(!file) {
        std::ofstream file(config_file_path);
        if (!file) {
            std::cerr << "Failed to create config file\n";
            return false;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        uuids::basic_uuid_random_generator<std::mt19937> uuid_gen{gen};
        uuids::uuid id = uuid_gen();
        std::string id_str = uuids::to_string(id);

        std::string json_template =
        "{\n"
        "    \"User\": {\n"
        "        \"AccessToken\": \"\",\n"
        "        \"UserId\": \"\",\n"
        "        \"UserName\": \"\",\n"
        "        \"ServerId\": \"\",\n"
        "        \"DeviceId\": \"" + id_str +"\"\n"
        "    }\n"
        "}";
        file << json_template;
        file.close();

        DeviceId = String::utf8(id_str.c_str());
        return true;
    } else {
    json config;
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

AppConfig::~AppConfig() {
    fprintf(stdout, "freeing up AppConfig");
}

void AppConfig::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_access_token"), &AppConfig::get_access_token);
    ClassDB::bind_method(D_METHOD("get_device_id"), &AppConfig::get_device_id);
    ClassDB::bind_method(D_METHOD("get_server_id"), &AppConfig::get_server_id);
    ClassDB::bind_method(D_METHOD("get_user_id"), &AppConfig::get_user_id);
    ClassDB::bind_method(D_METHOD("get_username"), &AppConfig::get_username);
}