#include "JellyAPI.h"

using namespace godot;

JellyAPI::JellyAPI() {};

void JellyAPI::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_collection_folders", "user_id", "callback"), &JellyAPI::get_collection_folders);
    ClassDB::bind_method(D_METHOD("fetch_item", "node", "item_id", "app_config", "network_config"), &JellyAPI::fetch_item);
    ClassDB::bind_method(D_METHOD("fetch_recently_added_items_from_collection", "node", "collection_id", "num_of_items", "app_config", "network_config"), &JellyAPI::fetch_recently_added_items_from_collection);
    ClassDB::bind_method(D_METHOD("fetch_item_image", "node", "item_id", "image_type", "width", "height", "app_config", "network_config"), &JellyAPI::fetch_item_image);
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


Ref<Json> JellyAPI::fetch_recently_added_items_from_collection(Node *node, String collection_id, int num_of_items, Ref<AppConfig> app_config, Ref<NetworkConfig> network_config) {
    if(num_of_items < 0 || num_of_items > 10) {
        UtilityFunctions::printerr("Number of items requested too large to fetch recently added content");
        Ref<Json> json = memnew(Json(node, network_config));
        return json;
    } else {
        String http_url = "http://" + network_config->get_server_url();
        String user_id = app_config->get_user_id();
        String endpoint = "/Items?ParentId=" + collection_id + "&Fields=Overview,Genres,RemoteTrailers&ExcludeItemTypes=Folder&SortBy=DateCreated&SortOrder=Descending&isPlayed=false&Limit=" + UtilityFunctions::str(num_of_items)  +"&Recursive=true&userId=" + user_id;
        String full_url = http_url + endpoint;

        Ref<Json> json = memnew(Json(node, network_config));
        json_requests.push_back(json);
        json->connect("request_completed_signal", Callable(this, "_on_json_request_completed").bind(json));
        json->json_get_request(full_url);

        return json;
    }
}

Ref<Json> JellyAPI::fetch_item(Node *node, String item_id, Ref<AppConfig> app_config, Ref<NetworkConfig> network_config) {
        String http_url = "http://" + network_config->get_server_url();
        String user_id = app_config->get_user_id();
        String endpoint = "/Users/" + user_id + "/Items/" + item_id + "?fields=MediaSources";
        String full_url = http_url + endpoint;

        Ref<Json> json = memnew(Json(node, network_config));
        json_requests.push_back(json);
        json->connect("request_completed_signal", Callable(this, "_on_json_request_completed").bind(json));
        json->json_get_request(full_url);

        return json;
}

Ref<Json> JellyAPI::fetch_item_image(Node *node, String item_id, String image_type, String width, String height, Ref<AppConfig> app_config, Ref<NetworkConfig> network_config) {
        String http_url = "http://" + network_config->get_server_url();
        String user_id = app_config->get_user_id();
        String endpoint = "/Items/" + item_id + "/Images/" + image_type +"?format=Webp&maxWidth=" + width + "&maxHeight=" + height;
        String full_url = http_url + endpoint;

        Ref<Json> json = memnew(Json(node, network_config));
        json_requests.push_back(json);
        json->connect("request_completed_signal", Callable(this, "_on_json_request_completed").bind(json));
        json->json_get_request(full_url);

        return json;
}

