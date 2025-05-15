#include <cstddef>
#include <string>
#include <iostream>
#include "Network.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);

    std::cout << "from write callback";
    return size * nmemb;
}
