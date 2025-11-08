#include "Settings.h"

Settings::Settings() {
    home.instantiate();
}

void Settings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_home"), &Settings::get_home);
    ClassDB::bind_method(D_METHOD("set_home", "new_home"), &Settings::set_home);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "home"), "set_home", "get_home");
}