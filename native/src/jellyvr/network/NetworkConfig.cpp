#include "NetworkConfig.h"
#include <string>
#include "../include/config.h"
#include "../include/version.h"

NetworkConfig::NetworkConfig() {
}

NetworkConfig::~NetworkConfig() {

}

void NetworkConfig::set_server_url(String url) {
    this->server_url = url;
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