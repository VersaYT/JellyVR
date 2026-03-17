extends Control

var jelly_api = JellyAPI.new();

@export var item_ref: Dictionary;
@export var image : ImageTexture;
@export var title : String;
@export var sub_info : String;
@export var item_id: String;
@export var progress: int;
@onready var title_node = $item_infos/title;
@onready var sub_info_node = $item_infos/sub_info;
@onready var content_progress_node = $item_infos/content_progress;
@onready var bg = $bg;

@onready var open_content_page_button = $open_content_page_button;


var play_btn_scene = preload("res://UI/Scenes/Content/play_button.tscn");
var play_btn;

func _ready() -> void:
	bg.clip_children = CLIP_CHILDREN_AND_DRAW;
	title_node.text = title;
	sub_info_node.text = sub_info;
	content_progress_node.value = progress;
	var image_request_node = Node.new();
	add_child(image_request_node);
	var imageWidth = "292";
	var imageHeight = "428";
	var imageType = "Thumb";

	var image_request = jelly_api.fetch_item_image(image_request_node, item_id, imageType, imageWidth, imageHeight, AppManager.config, AppManager.network);
	image_request.connect("received_image", Callable(self, "_on_image_received"));
	
	play_btn = play_btn_scene.instantiate();
	play_btn.visible = false;
	play_btn.mouse_filter = MOUSE_FILTER_PASS;
	play_btn.position = Vector2(53, 72);
	$".".add_child(play_btn);
	
	play_btn.connect("pressed", Callable(self, "_on_play_btn_pressed").bind(item_ref));

func _on_image_received(texture):
	var content_cover = $bg/content_cover;
	content_cover.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
	content_cover.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_COVERED;
	content_cover.texture = texture;

func _on_play_btn_pressed(item):
	var item_request_node = Node.new();
	add_child(item_request_node);
	
	var item_request = jelly_api.fetch_item(item_request_node, item["Id"], AppManager.config, AppManager.network);
	item_request.connect("request_completed_signal", Callable(self, "_on_item_received"));

func _on_mouse_entered() -> void:
	play_btn.visible = true;
	open_content_page_button.visible = true;

func _on_mouse_exited() -> void:
	play_btn.visible = false;
	open_content_page_button.visible = false;

func _on_item_received(item):
	var duration_in_ticks = item["MediaSources"][0]["RunTimeTicks"];
	var duration_in_seconds = duration_in_ticks / 10_000_000.0;
	StateMachine.duration_update(duration_in_seconds);
	var data = UIPlayerState.new();
	data.play_state = UIPlayerState.PlayState.PLAY;
	data.item = item;
	StateMachine.change_player_ui_state(data);
