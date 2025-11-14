#ifndef HOME_H
#define HOME_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class Home : public RefCounted {
    GDCLASS(Home, RefCounted);
private:
    int CarouselContentSelector {0};
    String ActiveCarouselCollectionFolderId {""};
public:
    int get_CarouselContentSelector() const {return CarouselContentSelector;}
    void set_CarouselContentSelector(int carousel_content_selector) {CarouselContentSelector = carousel_content_selector;}

    String get_ActiveCarouselCollectionFolderId() {return ActiveCarouselCollectionFolderId;}
    void set_ActiveCarouselCollectionFolderId(String id) {ActiveCarouselCollectionFolderId = id;}
protected:
    static void _bind_methods();
};

#endif