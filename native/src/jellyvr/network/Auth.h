#ifndef AUTH_H
#define AUTH_H

// #include "User.h"
#include <curl/curl.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <optional>
#include <string>
#include "../AppConfig.h"
#include "NetworkConfig.h"
#include <godot_cpp/classes/http_request.hpp>
#include "Json.h"

using namespace godot;

class Auth : public RefCounted {
    GDCLASS(Auth, RefCounted);

public:
    Auth();
    virtual ~Auth() override;

    void set_is_logged_in(bool value);

    bool ping(String url);

    bool login(String Username, String Pw, Ref<AppConfig> config, Ref<NetworkConfig> network_config);
    bool logout(Ref<AppConfig> config);
    // const User& get_user() const {return *m_user;}

    Ref<Json> get_public_users(Node *node, Ref<NetworkConfig> network_config);
    Ref<Json> get_user_profile_pic(Node *node, String user_id, Ref<NetworkConfig> network_config);
    bool get_is_logged_in();

    static void _bind_methods();
private:
    Vector<Ref<Json>> json_requests;
    void _on_json_request_completed(Variant, Ref<Json>);
    // std::optional<User> m_user;
    bool is_logged_in {false};
};

#endif