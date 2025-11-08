#include "NetworkConfig.h"
#include <string>
#include "../include/config.h"
#include "../include/version.h"
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/string.hpp>

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

NetworkConfig::NetworkConfig() {
}

NetworkConfig::~NetworkConfig() {

}

void NetworkConfig::set_server_url(String url) {
    this->server_url = url;
    
    String user_dir = ProjectSettings::get_singleton()->globalize_path("user://");
    String ConfigFilePath = user_dir + "Config/" + "JellyVR.json";

    fs::path config_file_path = ConfigFilePath.utf8().get_data();
    json config;
    std::ifstream file(config_file_path);
    file >> config;
    file.close();

    config["Network"]["ServerUrl"] = url.utf8().get_data();

    std::ofstream updated_file(ConfigFilePath.utf8().get_data());

    updated_file << config.dump(4);

    updated_file.close();
}
String NetworkConfig::get_server_url() {
    return this->server_url;
}
String NetworkConfig::get_authorization_header() {
    return this->authorization_header;
}

void NetworkConfig::set_authorization_header(String DeviceId) {
    std::string auth_header = ("Authorization: MediaBrowser ");
    auth_header.append("Client=\"").append(CLIENT_NAME).append("\", ");
    auth_header.append("Version=\"").append(PROJECT_VERSION).append("\", ");
    auth_header.append("DeviceId=\"").append(DeviceId.utf8().get_data()).append("\", ");
    auth_header.append("Device=\"").append(DEVICE_NAME).append("\"");

    this->authorization_header = String(auth_header.c_str());
}

void NetworkConfig::set_token_in_auth_header(String token) {
    std::string auth_header = this->authorization_header.utf8().get_data();
    auth_header.append(", Token=\"").append(token.utf8().get_data()).append("\"");

    UtilityFunctions::print(auth_header.c_str());

    this->authorization_header = String(auth_header.c_str());
}

void NetworkConfig::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_server_url", "url"), &NetworkConfig::set_server_url);
    ClassDB::bind_method(D_METHOD("get_server_url"), &NetworkConfig::get_server_url);
}