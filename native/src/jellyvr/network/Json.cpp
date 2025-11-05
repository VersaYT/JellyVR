#include "Json.h"
#include "../include/config.h"
#include "../utils/Network.h"

#include <godot_cpp/classes/http_request.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/json.hpp>

using namespace godot;

// Json::Json() {
//     UtilityFunctions::print("Json constructor called");
//     // http_request = memnew(HTTPRequest);
// }

// Json::~Json() {
//     fprintf(stdout, "Freeing Json");
// }

void Json::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_request_completed", "result", "response_code", "headers", "body"), &Json::_on_request_completed);
    ClassDB::bind_method(D_METHOD("json_get_request", "url"), &Json::json_get_request);
    ADD_SIGNAL(MethodInfo("request_completed_signal", PropertyInfo(Variant::ARRAY, "result")));
    ADD_SIGNAL(MethodInfo(
    "received_image",
    PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, "ImageTexture")
));
}

Json::Json(Node *node, Ref<NetworkConfig> config) {
    UtilityFunctions::print("calling Json constructor");
    this->network_config = config;

    http_request = memnew(HTTPRequest);
    node->add_child(http_request);

    this->headers.push_back("Content-Type: application/json");
    this->headers.push_back(this->network_config->get_authorization_header());
    http_request->connect("request_completed", Callable(this, "_on_request_completed"));

}

void Json::json_get_request(const String &url) {
    UtilityFunctions::print("Requesting: ", url);
    Error err = http_request->request(url, this->headers, HTTPClient::METHOD_GET);
    UtilityFunctions::print("Request error code: ", err);
}

void Json::_on_request_completed(int result, int response_code, PackedStringArray headers, PackedByteArray body) {

    if (result != HTTPRequest::RESULT_SUCCESS || response_code != 200) {
        UtilityFunctions::print("Request failed: ", response_code);
        // throw "Failed to fetch image. HTTP code: " + response_code;
    }

    Dictionary headers_dict = parse_headers_to_dict(headers);

    if(headers_dict["Content-Type"] == "application/json; charset=utf-8") {
    String body_str = String::utf8(reinterpret_cast<const char *>(body.ptr()), body.size());

    Variant parsed = JSON::parse_string(body_str);

    Array raw_array = parsed;
    Array dict_array;

    for (int i = 0; i < raw_array.size(); i++) {
        Variant item = raw_array[i];
        if (item.get_type() == Variant::DICTIONARY) {
            dict_array.append(item); // Append Dictionary to Array
        } else {
            UtilityFunctions::printerr("Warning: JSON array contains non-dictionary element at index ", i);
        }
    }
 
    this->request_result = dict_array;
    emit_signal("request_completed_signal", dict_array);
}
    else if(headers_dict["Content-Type"] == "image/webp") {
        Ref<Image> image = memnew(Image);

        Error err = image->load_webp_from_buffer(body);

        if (err != OK) {
            UtilityFunctions::printerr("Failed to load image from buffer");
            return;
        }

        Ref<ImageTexture> texture = memnew(ImageTexture);
        texture->set_image(image);

        emit_signal("received_image", texture);
    } else {
        UtilityFunctions::printerr("Content-type not supported at the moment, like what, what are you trying to get my guy");
    }


}
