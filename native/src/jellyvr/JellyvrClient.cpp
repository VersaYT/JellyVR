#include "JellyvrClient.h"
#include <string>
#include <stdio.h>

JellyvrClient::JellyvrClient() {
    m_auth.instantiate();
    m_auth->set_server_provider(this);
}
JellyvrClient::~JellyvrClient() {
    fprintf(stdout, "freeing jellyvr client");
}

void JellyvrClient::_bind_methods() {
    ClassDB::bind_method(D_METHOD("auth"), &JellyvrClient::auth);
    ClassDB::bind_method(D_METHOD("set_server_url"), &JellyvrClient::set_server_url);
}

Ref<Auth> JellyvrClient::auth() {
    return m_auth;
}

std::string JellyvrClient::get_server_url() const{
    return server_url;
}

void JellyvrClient::set_server_url(String url) {
    String result = m_auth->ping(url);
    std::string converted_str = result.utf8().get_data();
        if(!converted_str.empty()) {
            server_url = url.utf8().get_data();
        } else {
            UtilityFunctions::print("Url was incorrect");
        }
}