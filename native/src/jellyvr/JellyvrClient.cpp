#include "JellyvrClient.h"
#include <string>
#include <stdio.h>

JellyvrClient::JellyvrClient() {
    UtilityFunctions::print("JellyVR constructor called");
    auth.instantiate();
    UtilityFunctions::print("Auth done");
    appConfig.instantiate();
    UtilityFunctions::print("Appconfig done");
    networkConfig.instantiate();
    UtilityFunctions::print("NetworkConfig done");

    networkConfig->set_authorization_header(appConfig->get_device_id());
}
JellyvrClient::~JellyvrClient() {
    UtilityFunctions::print("Freeing JellyVRClient");
}

void JellyvrClient::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_Auth"), &JellyvrClient::get_Auth);
    ClassDB::bind_method(D_METHOD("get_AppConfig"), &JellyvrClient::get_AppConfig);
    ClassDB::bind_method(D_METHOD("get_NetworkConfig"), &JellyvrClient::get_NetworkConfig);
}