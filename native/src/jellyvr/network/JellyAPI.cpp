#include "JellyAPI.h"

using namespace godot;

JellyAPI::JellyAPI() {};

void JellyAPI::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_collection_folders", "user_id", "callback"), &JellyAPI::get_collection_folders);
    ClassDB::bind_method(D_METHOD("_on_json_request_completed"), &JellyAPI::_on_json_request_completed);
}

void JellyAPI::_on_json_request_completed(Variant data, Ref<Json> json) {
    UtilityFunctions::print("Freeing json request");
    json_requests.erase(json);
}

Ref<Json> JellyAPI::get_collection_folders(Node *node, Ref<AppConfig> app_config, Ref<NetworkConfig> network_config) {
    String http_url = "http://" + network_config->get_server_url();
    String user_id = app_config->get_user_id();
    String endpoint = "/Items?userId=" + user_id;
    String full_url = http_url + endpoint;

    Ref<Json> json = memnew(Json(node, network_config));
    json_requests.push_back(json);
    json->connect("request_completed_signal", Callable(this, "_on_json_request_completed").bind(json));
    json->json_get_request(full_url);

    return json;
}