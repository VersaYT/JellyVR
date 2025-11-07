#ifndef JELLY_API_H
#define JELLY_API_H

#include <godot_cpp/classes/ref_counted.hpp>
#include "NetworkConfig.h"
#include "../AppConfig.h"
#include "Json.h"

using namespace godot;

class JellyAPI : public RefCounted {
    GDCLASS(JellyAPI, RefCounted);
private:
    Vector<Ref<Json>> json_requests;

protected:
    static void _bind_methods();

public:
    JellyAPI();

    void _on_json_request_completed(Variant data, Ref<Json> json);
    Ref<Json> get_collection_folders(Node *node, Ref<AppConfig> app_config, Ref<NetworkConfig> network_config);
};

#endif