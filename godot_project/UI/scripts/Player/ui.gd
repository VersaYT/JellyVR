extends Control

@onready var play_pause_button : Button = $items_container/play_pause_button;
@onready var items_container = $items_container;
var play_icon = preload("res://UI/assets/img/Player/Play.png");
var pause_icon = preload("res://UI/assets/img/Player/Pause.png");

func _ready() -> void:
	var transparent_bg := $transparent_bg;
	StateMachine.connect("UIPlayerRequest", Callable(self, "_on_ui_player_request"));
	transparent_bg.connect("gui_input", Callable(self, "_on_ui_click"));
	transparent_bg.connect("mouse_exited", Callable(self, "_on_ui_mouse_exit"));
	play_pause_button.connect("pressed", Callable(self, "_on_play_pause_button_pressed"));

func _on_ui_click(event) -> void:
	if event is InputEventMouseButton: 
		if event.button_index == MOUSE_BUTTON_LEFT and event.pressed and StateMachine.current_ui_player_state != null:
			if StateMachine.current_ui_player_state.play_state != UIPlayerState.PlayState.STOP:
				items_container.visible = true;
				StateMachine.emit_signal("ToggleUIFloatingButtons", true);
func _on_play_pause_button_pressed() -> void:
	var data := UIPlayerState.new();
	if StateMachine.current_ui_player_state.play_state == UIPlayerState.PlayState.PLAY || StateMachine.current_ui_player_state.play_state == UIPlayerState.PlayState.RESUME:
		play_pause_button.icon = play_icon;
		data.play_state = UIPlayerState.PlayState.PAUSE;
		StateMachine.change_player_ui_state(data);
	else:
		play_pause_button.icon = pause_icon;
		data.play_state = UIPlayerState.PlayState.RESUME;
		StateMachine.change_player_ui_state(data);
		items_container.visible = false;
		StateMachine.emit_signal("ToggleUIFloatingButtons", false);

func _on_ui_player_request(data: UIPlayerState) -> void:
	if data.play_state == UIPlayerState.PlayState.PLAY:
		var content_title = $items_container/content_title;
		content_title.text = StateMachine.current_ui_player_state.item["Name"];
