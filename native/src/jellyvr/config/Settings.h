#ifndef SETTINGS_H
#define SETTINGS_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "Home.h"

using namespace godot;

class Settings : public RefCounted {
    GDCLASS(Settings, RefCounted);
private:
    Ref<Home> home;
protected:
    static void _bind_methods();
public:
    Settings();
    Ref<Home> get_home() const {return home;}
    void set_home(Ref<Home> new_home) {home = new_home;}
};


#endif