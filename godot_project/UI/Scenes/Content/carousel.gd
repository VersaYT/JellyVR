extends Control

var jelly_api = JellyAPI.new();
const Utils = preload("res://UI/scripts/utils.gd");
var carousel_item_scene = preload("res://UI/Scenes/Content/Carousel/CarouselItem.tscn");
@onready var carousel_item_container = $Panel/ScrollContainer/carousel_item_container;
@onready var scroll_container: ScrollContainer = $Panel/ScrollContainer;
var ignore_scroll_events := false
var active_item_index := 0;
var scroll_start : Vector2;
var scroll_end : Vector2;
var number_of_requested_items: int = 10;

func _ready() -> void:
	scroll_container.connect("scroll_started", Callable(self, "_on_scroll_started"));
	scroll_container.connect("scroll_ended", Callable(self, "_on_scroll_ended"));
	var parent = get_parent()
	parent.connect("CollectionFolderIdSet", Callable(self, "_on_collection_folder_id_set"))
	if AppManager.config.settings.home.get_ActiveCarouselCollectionFolderId() != "":
		var request = Node.new();
		add_child(request);
		var content_dict = jelly_api.fetch_recently_added_items_from_collection(request, AppManager.config.settings.home.ActiveCarouselCollectionFolderId, number_of_requested_items, AppManager.config, AppManager.network);
		content_dict.connect("request_completed_signal", Callable(self, "_on_content_received"));

func _on_content_received(content):
	for item in content["Items"]:
		var carousel_item = carousel_item_scene.instantiate();
		carousel_item.item_ref = item;
		carousel_item.title = item["Name"];
		if item.has("CommunityRating"):
			carousel_item.score = str(Utils.convert_score_10_to_5(float(item["CommunityRating"])));
		carousel_item.item_id = item["Id"];
		var categories_arr = item["Genres"];
		carousel_item.categories = String(", ").join(categories_arr.map(str))
		if item.has("Overview"):
			carousel_item.description = item["Overview"];
		carousel_item_container.add_child(carousel_item);

func _on_collection_folder_id_set():
	var request = Node.new();
	add_child(request);
	var content_dict = jelly_api.fetch_recently_added_items_from_collection(request, AppManager.config.settings.home.ActiveCarouselCollectionFolderId, number_of_requested_items, AppManager.config, AppManager.network);
	content_dict.connect("request_completed_signal", Callable(self, "_on_content_received"));
func _on_scroll_started():
	if ignore_scroll_events:
		return
	scroll_start = get_viewport().get_mouse_position()

func _on_scroll_ended():
	if ignore_scroll_events:
		return
	scroll_end = get_viewport().get_mouse_position()
	if scroll_start.x > scroll_end.x:
		if active_item_index < number_of_requested_items:
			active_item_index += 1; 
	elif scroll_start.x < scroll_end.x:
		if active_item_index > 0:
			active_item_index -= 1;

	animate_scrolling()

func animate_scrolling():
	ignore_scroll_events = true

	var tween := get_tree().create_tween();
	tween.tween_property(scroll_container, "scroll_horizontal", active_item_index * 621, 0.2);
	tween.finished.connect(func(): ignore_scroll_events = false)
