#pragma once
#include <curl.h>

#ifdef __cplusplus
extern "C" {
#endif

int my_curl_debug_callback(CURL *handle, curl_infotype type,
                        char *data, size_t size, void *userptr);

#ifdef __cplusplus
}
#endif