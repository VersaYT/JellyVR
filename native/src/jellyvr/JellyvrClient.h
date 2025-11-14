#ifndef JELLYVRCLIENT_H
#define JELLYVRCLIENT_H

#include <godot_cpp/classes/ref_counted.hpp>
#include "network/Auth.h"
#include "AppConfig.h"
#include "network/NetworkConfig.h"
#include <string>
#include "network/Json.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

using namespace godot;

class JellyvrClient : public RefCounted{
    GDCLASS(JellyvrClient, RefCounted);

private:
    Ref<Auth> auth;
    Ref<AppConfig> appConfig;
    Ref<NetworkConfig> networkConfig;
    // Ref<Json> json;
public:
    JellyvrClient();
    virtual ~JellyvrClient() override;

    Ref<Auth> get_Auth() const { return auth; }
    Ref<AppConfig> get_AppConfig() const { return appConfig; }
    Ref<NetworkConfig> get_NetworkConfig() const { return networkConfig; }

    // void _ready();

    void populate_settings_from_config_file();
    void clean_server_related_config();

protected:
	static void _bind_methods();

};

#endif