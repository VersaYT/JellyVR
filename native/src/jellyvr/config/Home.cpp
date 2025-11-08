#include "Home.h"

void Home::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_CarouselContentSelector"), &Home::get_CarouselContentSelector);
    ClassDB::bind_method(D_METHOD("set_CarouselContentSelector", "CarouselContentSelector"), &Home::set_CarouselContentSelector);
    ClassDB::bind_method(D_METHOD("get_ActiveCarouselCollectionFolderId"), &Home::get_ActiveCarouselCollectionFolderId);
    ClassDB::bind_method(D_METHOD("set_ActiveCarouselCollectionFolderId", "id"), &Home::set_ActiveCarouselCollectionFolderId);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "CarouselContentSelector"), "set_CarouselContentSelector", "get_CarouselContentSelector");

}