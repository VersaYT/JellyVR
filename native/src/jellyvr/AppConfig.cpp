#include "AppConfig.h"
#include "utils/ConfigPath.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include "third_party/stduuid/uuid.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

AppConfig::AppConfig() {
namespace fs = std::filesystem;

fs::path config_file_path = getConfigPath("JellyVR");

std::ifstream file(config_file_path);

// generate a new id if config file doesnt exist
    if(!file) {
        std::ofstream file(config_file_path);
        if (!file) {
            std::cerr << "Failed to create config file\n";
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
    } else {
    json config;
    try {
        file >> config;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }

    std::cout << "Parsed config:\n" << config.dump(4) << std::endl;

    std::string c_DeviceId = config["User"]["DeviceId"];
    DeviceId = String::utf8(c_DeviceId.c_str()) ;
    }
}

AppConfig::~AppConfig() {
    fprintf(stdout, "freeing up AppConfig");
}