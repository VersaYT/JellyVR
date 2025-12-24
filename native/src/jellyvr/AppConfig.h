#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include "config/Settings.h"
#include "include/version.h"
#include "structs/AppConfig.h"
#include <nlohmann/json.hpp>

using namespace godot;
using json = nlohmann::json;

class AppConfig : public RefCounted{
    GDCLASS(AppConfig, RefCounted);
    
public:
    AppConfig();
    virtual ~AppConfig() override;

    void initSSL();
    void initPaths();
    void initialize();

    void init();

    void set_settings(Ref<Settings> new_settings) {settings = new_settings;}
    Ref<Settings> get_settings() {return settings;}

    void set_collection_folders(Array collection) {collection_folders = collection;}
    Array get_collection_folders() const {return collection_folders;}

    String get_access_token() const {return AccessToken;}
    String get_device_id() const {return DeviceId;}
    String get_server_id() const {return ServerId;}
    String get_user_id() const {return UserId;}
    String get_username() const {return UserName;}

    String get_app_version() {return PROJECT_VERSION;}
    String get_yt_dlp_binary_name() {return yt_dlp_binary_name;}
    void set_yt_dlp_binary_name(String name) {yt_dlp_binary_name = name;}

    bool fetch_yt_dlp();
    ReturnedData fetch_github_direct_download_link(std::string repo_path, std::string target_platform);


    void set_access_token(String token) {AccessToken = token;}
    void set_device_id(String device_id) {DeviceId = device_id;}
    void set_server_id(String server_id) {ServerId = server_id;}
    void set_user_id(String user_id) {UserId = user_id;}
    void set_username(String username) {UserName = username;}

    void set_config_value(PackedStringArray keys, Variant value);
    void migrate_config_file(const json &old_config, json &new_config);

    String ConfigPath;
    String CachePath;
    String LogsPath;
    String BinPath;
    String TmpPath;
    String SSLPath;
    String ConfigFilePath;

private:

    Ref<Settings> settings;

    String AccessToken;
    String DeviceId;
    String ServerId;
    String UserId;
    String UserName;

    Array collection_folders;

    String yt_dlp_binary_name;

    bool initConfigFile();

protected:
    static void _bind_methods();

};

#endif