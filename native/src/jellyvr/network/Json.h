#ifndef JSON_H
#define JSON_H

#include <godot_cpp/classes/http_request.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "NetworkConfig.h"

using namespace godot;

class Json : public RefCounted {
    GDCLASS(Json, RefCounted);

private:
    HTTPRequest *http_request = nullptr;
    Ref<NetworkConfig> network_config;

public:
    Json() = default;
    ~Json() = default;
    Json(Node *node, Ref<NetworkConfig> config);
    PackedStringArray headers {};
    Variant request_result {};

    void json_get_request(const String &url);

    void _on_request_completed(int result, int response_code, PackedStringArray headers, PackedByteArray body);

    static void _bind_methods();
};

#endif