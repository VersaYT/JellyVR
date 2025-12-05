#include "DebugCurl.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <string>

using namespace godot;

int my_curl_debug_callback(CURL *handle, curl_infotype type,
                        char *data, size_t size, void *userptr)
{
    std::string s(data, size);
    UtilityFunctions::print(String(s.c_str()));
    return 0;
}