#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class AppConfig : public RefCounted {
    GDCLASS(AppConfig, RefCounted);

public:
    AppConfig();
    ~AppConfig();

    String AccessToken;
    String DeviceId;
    String ServerId;
    String UserId;
    String UserName;
private:

protected:
    static void _bind_methods() {
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "AccessToken"), "", "");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "DeviceId"), "", "");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "ServerId"), "", "");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "UserId"), "", "");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "UserName"), "", "");
    }

};

#endif