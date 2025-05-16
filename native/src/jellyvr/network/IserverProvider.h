#ifndef ISERVERPROVIDER_H
#define ISERVERPROVIDER_H

#include <string>

class IServerProvider {
    public:
        virtual ~IServerProvider() = default;
        virtual std::string get_server_url() const = 0;
    };

#endif