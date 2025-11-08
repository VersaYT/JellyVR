#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include "config/Settings.h"

using namespace godot;

class AppConfig : public RefCounted{
    GDCLASS(AppConfig, RefCounted);
    
public:
    AppConfig();
    virtual ~AppConfig() override;

    void initPaths();
    void initialize();

    void set_settings(Ref<Settings> new_settings) {settings = new_settings;}
    Ref<Settings> get_settings() {return settings;}

    void set_collection_folders(Array collection) {collection_folders = collection;}
    Array get_collection_folders() const {return collection_folders;}

    String get_access_token() const {return AccessToken;}
    String get_device_id() const {return DeviceId;}
    String get_server_id() const {return ServerId;}
    String get_user_id() const {return UserId;}
    String get_username() const {return UserName;}


    void set_access_token(String token) {AccessToken = token;}
    void set_device_id(String device_id) {DeviceId = device_id;}
    void set_server_id(String server_id) {ServerId = server_id;}
    void set_user_id(String user_id) {UserId = user_id;}
    void set_username(String username) {UserName = username;}

    void set_config_value(PackedStringArray keys, Variant value);

    String ConfigPath;
    String CachePath;
    String LogsPath;
    String ConfigFilePath;

private:

    Ref<Settings> settings;

    String AccessToken;
    String DeviceId;
    String ServerId;
    String UserId;
    String UserName;

    Array collection_folders;

    bool initConfigFile();

protected:
    static void _bind_methods();

};

#endif