#ifndef AUTH_H
#define AUTH_H

#include "User.h"
#include <curl/curl.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <optional>
#include <string>
#include "network/IserverProvider.h"

using namespace godot;

class JellyvrClient;

class Auth : public RefCounted {
    GDCLASS(Auth, RefCounted);

public:
    Auth();
    ~Auth();

    void set_server_provider(IServerProvider*);

    String ping(String url);

    bool login(String Username, String Pw);
    bool logout();
    const User& get_user() const {return *m_user;}

    static void _bind_methods();
private:
    IServerProvider* server_provider = nullptr;
    std::optional<User> m_user;
};

#endif