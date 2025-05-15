#ifndef AUTH_H
#define AUTH_H

#include "User.h"
#include <curl/curl.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <optional>
#include <string>

using namespace godot;

class Auth : public RefCounted {
    GDCLASS(Auth, RefCounted);

public:
    Auth();
    ~Auth();

    String ping(String url);

    bool login(const String& name, const String& passw);
    bool logout();
    const User& get_user() const {return *m_user;}

    static void _bind_methods();
private:
    std::optional<User> m_user;
};

#endif