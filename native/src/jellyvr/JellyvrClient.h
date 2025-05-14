#ifndef JELLYVRCLIENT_H
#define JELLYVRCLIENT_H

#include <godot_cpp/classes/ref_counted.hpp>
#include "Auth.h"
#include <string>

using namespace godot;

class JellyvrClient : public RefCounted {
    GDCLASS(JellyvrClient, RefCounted)

public:
    JellyvrClient();
    ~JellyvrClient();

    Ref<Auth> auth();

    const std::string& get_server_url() const;
    void set_server_url(const std::string& url);
private:
    Ref<Auth> m_auth;
    std::string server_url;
protected:
	static void _bind_methods();



};

#endif