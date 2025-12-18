extends Control

var jelly_api = JellyAPI.new();

@export var item_ref: Dictionary;

@export var image : ImageTexture;
@export var title : String;
@export var categories : String;
@export var description: String;
@export var score: String;
@export var item_id: String;
@onready var panel = $Panel;

@onready var title_node = $content_infos/title;
@onready var categories_node = $content_infos/categories;
@onready var description_node = $content_infos/description;
@onready var score_node = $content_infos/rating/score;
@onready var rating = $content_infos/rating;

func _ready() -> void:
	panel.clip_children = CLIP_CHILDREN_AND_DRAW;
	title_node.text = title;
	categories_node.text = categories;
	description_node.text = description;
	score_node.text = score;
	var trailer_button = $trailer_button;
	var image_request_node = Node.new();
	add_child(image_request_node);
	var imageWidth = "621";
	var imageHeight = "235";
	var imageType : String;
	var contentType = item_ref["Type"];
	if item_ref.has("RemoteTrailers") and item_ref["RemoteTrailers"] != []:
		trailer_button.visible = true;
	
	match contentType:
		"Movie":
			imageType = "Backdrop"
		"Series":
			imageType = "Primary"
		"Episode":
			imageType = "Primary"
	
	var image_request = jelly_api.fetch_item_image(image_request_node, item_id, imageType, imageWidth, imageHeight, AppManager.config, AppManager.network);
	image_request.connect("received_image", Callable(self, "_on_image_received"));
	
	var stars_scene = load("res://UI/Scenes/Content/stars.tscn");
	var stars = stars_scene.instantiate();
	stars.score = score;
	
	rating.add_child(stars);
	rating.move_child(stars, 0);
	
	var play_btn_scene = load("res://UI/Scenes/Content/play_button.tscn");
	var play_btn = play_btn_scene.instantiate();
	play_btn.position = Vector2(319, 97);
	$".".add_child(play_btn);
	
	play_btn.connect("pressed", Callable(self, "_on_play_btn_pressed").bind(item_ref));

func _on_image_received(texture):
	var content_cover = $Panel/content_cover;
	content_cover.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
	content_cover.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_COVERED;
	content_cover.texture = texture;

func _on_play_btn_pressed(item):
	var item_request_node = Node.new();
	add_child(item_request_node);
	
	var item_request = jelly_api.fetch_item(item_request_node, item["Id"], AppManager.config, AppManager.network);
	item_request.connect("request_completed_signal", Callable(self, "_on_item_received"));

func _on_trailer_button_pressed():
	var data = UIPlayerState.new();
	data.play_state = UIPlayerState.PlayState.PLAY;
	data.item = item_ref;
	data.trailer_request = true;
	StateMachine.change_player_ui_state(data);

func _on_item_received(item):
	var duration_in_ticks = item["MediaSources"][0]["RunTimeTicks"];
	var duration_in_seconds = duration_in_ticks / 10_000_000.0;
	StateMachine.duration_update(duration_in_seconds);
	var data = UIPlayerState.new();
	data.play_state = UIPlayerState.PlayState.PLAY;
	data.item = item;
	StateMachine.change_player_ui_state(data);
