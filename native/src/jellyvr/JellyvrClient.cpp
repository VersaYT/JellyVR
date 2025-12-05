#include "JellyvrClient.h"
#include <string>
#include <stdio.h>

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

JellyvrClient::JellyvrClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    UtilityFunctions::print("JellyVR constructor called");
    auth.instantiate();
    UtilityFunctions::print("Auth done");
    appConfig.instantiate();
    UtilityFunctions::print("Appconfig done");
    networkConfig.instantiate();
    UtilityFunctions::print("NetworkConfig done");
}
JellyvrClient::~JellyvrClient() {
    UtilityFunctions::print("Freeing JellyVRClient");
}

void JellyvrClient::populate_settings_from_config_file() {
    UtilityFunctions::print("populating settings from config file");
    fs::path config_file_path = this->appConfig->ConfigFilePath.utf8().get_data();
    json config;
    std::ifstream file(config_file_path);
    file >> config;

    string access_token = config["User"]["AccessToken"];
    string device_id = config["User"]["DeviceId"];
    string server_id = config["User"]["ServerId"];
    string user_id = config["User"]["UserId"];
    string username = config["User"]["UserName"];

    string url = config["Network"]["ServerUrl"];

    int carousel_content_selector = config["Settings"]["Home"]["CarouselContentSelector"];
    string ActiveCarouselCollectionFolderId = config["Settings"]["Home"]["ActiveCarouselCollectionFolderId"];

    this->appConfig->set_access_token(access_token.c_str());
    this->appConfig->set_server_id(server_id.c_str());
    this->appConfig->set_user_id(user_id.c_str());
    this->appConfig->set_username(username.c_str());

    this->networkConfig->set_server_url(url.c_str());
    this->networkConfig->set_authorization_header(appConfig->get_device_id());
    this->networkConfig->set_token_in_auth_header(access_token.c_str());

    Ref<Settings> settings = this->appConfig->get_settings();
    Ref<Home> home = settings->get_home();

    home->set_CarouselContentSelector(carousel_content_selector);
    home->set_ActiveCarouselCollectionFolderId(ActiveCarouselCollectionFolderId.c_str());
        // this needs implementing to check wether the token is still valid
    if(!access_token.empty()) {
        this->auth->set_is_logged_in(true);
    }
    file.close();
}

void JellyvrClient::clean_server_related_config() {
    UtilityFunctions::print("cleaning server related config items");

    fs::path config_file_path = this->appConfig->ConfigFilePath.utf8().get_data();
    json config;
    std::ifstream file(config_file_path);
    file >> config;
    file.close();

    config["Network"]["ServerUrl"] = "";
    config["User"]["ServerId"] = "";

    config["Settings"]["Home"]["CarouselContentSelector"] = 0;
    config["Settings"]["Home"]["ActiveCarouselCollectionFolderId"] = "";

    std::ofstream new_file(config_file_path);
    new_file << config.dump(4);
    new_file.close();
}

void JellyvrClient::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_Auth"), &JellyvrClient::get_Auth);
    ClassDB::bind_method(D_METHOD("get_AppConfig"), &JellyvrClient::get_AppConfig);
    ClassDB::bind_method(D_METHOD("get_NetworkConfig"), &JellyvrClient::get_NetworkConfig);
    ClassDB::bind_method(D_METHOD("populate_settings_from_config_file"), &JellyvrClient::populate_settings_from_config_file);
    ClassDB::bind_method(D_METHOD("clean_server_related_config"), &JellyvrClient::clean_server_related_config);
}