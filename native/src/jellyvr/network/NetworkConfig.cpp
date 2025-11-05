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
    std::string authorization_header = ("Authorization: MediaBrowser ");
    authorization_header.append("Client=\"").append(CLIENT_NAME).append("\", ");
    authorization_header.append("Version=\"").append(PROJECT_VERSION).append("\", ");
    authorization_header.append("DeviceId=\"").append(DeviceId.utf8().get_data()).append("\", ");
    authorization_header.append("Device=\"").append(DEVICE_NAME).append("\" ");

    this->authorization_header = String(authorization_header.c_str());
}

void NetworkConfig::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_server_url", "url"), &NetworkConfig::set_server_url);
    ClassDB::bind_method(D_METHOD("get_server_url"), &NetworkConfig::get_server_url);
}