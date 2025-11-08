extends Control

@onready var carousel_content_selector = $carousel_content_selector;
@onready var carousel_content_selector_last_selected;
func _ready() -> void:
	populate_carousel_content_selector();
	carousel_content_selector.selected = AppManager.config.settings.home.CarouselContentSelector;
	carousel_content_selector_last_selected = AppManager.config.settings.home.CarouselContentSelector;

func _on_carousel_content_selector_item_selected(index: int) -> void:
	if carousel_content_selector_last_selected == index:
		return;
	else:
		var keys = ["Settings", "Home", "CarouselContentSelector"];
		AppManager.config.set_config_value(keys, index);
		AppManager.config.settings.home.set_CarouselContentSelector(index);
		
		var collection = find_dict_by_key_value(AppManager.config.get_collection_folders(), "Name", carousel_content_selector.get_item_text(carousel_content_selector.selected));
		
		keys = ["Settings", "Home", "ActiveCarouselCollectionFolderId"];
		AppManager.config.set_config_value(keys, collection["Id"]);
		AppManager.config.settings.home.set_ActiveCarouselCollectionFolderId(collection["Id"]);
		carousel_content_selector_last_selected = carousel_content_selector.selected;
		
func find_dict_by_key_value(array, key, value):
	for dict in array:
		if dict[key] == value:
			return dict;

func populate_carousel_content_selector():
	var collection_folders = AppManager.config.get_collection_folders();
	var i = 0;
	for collection in collection_folders:
		carousel_content_selector.add_item(collection["Name"], i);
		i += 1;
