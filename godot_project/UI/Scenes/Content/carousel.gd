extends Control

var jelly_api = JellyAPI.new();
const Utils = preload("res://UI/scripts/utils.gd");

func _ready() -> void:
	var parent = get_parent()
	parent.connect("CollectionFolderIdSet", Callable(self, "_on_collection_folder_id_set"))
	if AppManager.config.settings.home.get_ActiveCarouselCollectionFolderId() != "":
		var request = Node.new();
		add_child(request);
		var content_dict = jelly_api.fetch_recently_added_items_from_collection(request, AppManager.config.settings.home.ActiveCarouselCollectionFolderId, 10, AppManager.config, AppManager.network);
		content_dict.connect("request_completed_signal", Callable(self, "_on_content_received"));

func _on_content_received(content):
	var title = $content_infos/title;
	var score = $content_infos/rating/score;
	var score_value;
	var categories = $content_infos/categories;
	var description = $content_infos/description;
	var rating = $content_infos/rating;
	
	var image_request_node = Node.new();
	add_child(image_request_node);
	
	var image_request = jelly_api.fetch_backdrop(image_request_node, content["Items"][0]["Id"], AppManager.config, AppManager.network);
	image_request.connect("received_image", Callable(self, "_on_image_received"));
	
	title.text = content["Items"][0]["Name"];
	score_value = str(Utils.convert_score_10_to_5(float(content["Items"][0]["CommunityRating"])));
	score.text = score_value;
	
	var categories_arr = content["Items"][0]["Genres"];
	categories.text = String(", ").join(categories_arr.map(str))
	
	description.text = content["Items"][0]["Overview"];
	
	var stars_scene = load("res://UI/Scenes/Content/stars.tscn");
	var stars = stars_scene.instantiate();
	stars.score = score_value;
	
	rating.add_child(stars);
	rating.move_child(stars, 0);
	
	var play_btn_scene = load("res://UI/Scenes/Content/play_button.tscn");
	var play_btn = play_btn_scene.instantiate();
	play_btn.position = Vector2(319, 97);
	$".".add_child(play_btn);
	
	play_btn.connect("pressed", Callable(self, "_on_play_btn_pressed").bind(content["Items"][0]));

func _on_image_received(texture):
	var content_cover = $Panel/content_cover;
	content_cover.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
	content_cover.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_COVERED;
	content_cover.texture = texture;

func _on_play_btn_pressed(item):
	print("button has been pressed");
	var item_request_node = Node.new();
	add_child(item_request_node);
	
	var item_request = jelly_api.fetch_item(item_request_node, item["Id"], AppManager.config, AppManager.network);
	item_request.connect("request_completed_signal", Callable(self, "_on_item_received"));

func _on_item_received(item):
	var data = UIPlayerState.new();
	data.play_state = UIPlayerState.PlayState.PLAY;
	data.item = item;
	StateMachine.change_player_ui_state(data);

func _on_collection_folder_id_set():
	var request = Node.new();
	add_child(request);
	var content_dict = jelly_api.fetch_recently_added_items_from_collection(request, AppManager.config.settings.home.ActiveCarouselCollectionFolderId, 10, AppManager.config, AppManager.network);
	content_dict.connect("request_completed_signal", Callable(self, "_on_content_received"));
