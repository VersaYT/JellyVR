extends Control

@onready var carousel_content_selector = $carousel_content_selector;

func _ready() -> void:
	carousel_content_selector.selected = AppManager.config.settings.home.CarouselContentSelector;


func _on_carousel_content_selector_item_selected(index: int) -> void:
	var keys = ["Settings", "Home", "CarouselContentSelector"];
	AppManager.config.set_config_value(keys, index);
	AppManager.config.settings.home.set_CarouselContentSelector(index);
