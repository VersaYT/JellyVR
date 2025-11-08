#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include "../AppConfig.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

using namespace godot;

class NetworkConfig : public RefCounted {
    GDCLASS(NetworkConfig, RefCounted);

private:
    String server_url {""};
    String authorization_header {""};

public:
    NetworkConfig();
    virtual ~NetworkConfig() override;

    void set_server_url(String url);
    void set_authorization_header(String DeviceId);
    String get_server_url();
    String get_authorization_header();
    void set_token_in_auth_header(String token);

    static void _bind_methods();
};

#endif