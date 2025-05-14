#include "JellyvrClient.h"
#include <string>
#include <stdio.h>

JellyvrClient::JellyvrClient() {
    m_auth.instantiate();
}
JellyvrClient::~JellyvrClient() {
    fprintf(stdout, "freeing jellyvr client");
}

void JellyvrClient::_bind_methods() {
    ClassDB::bind_method(D_METHOD("auth"), &JellyvrClient::auth);
}

Ref<Auth> JellyvrClient::auth() {
    return m_auth;
}

const std::string& JellyvrClient::get_server_url() const{
    return server_url;
}

void JellyvrClient::set_server_url(const std::string &url) {
    server_url = url;
}