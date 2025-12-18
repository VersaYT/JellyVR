extends Control

@onready var play_pause_button : Button = $items_container/play_pause_button;
@onready var items_container = $items_container;
@onready var content_timeline := $items_container/content_timeline;
@onready var content_elapsed_time := $items_container/content_elapsed_time;
var play_icon = preload("res://UI/assets/img/Player/Play.png");
var pause_icon = preload("res://UI/assets/img/Player/Pause.png");

func _ready() -> void:
	var transparent_bg := $transparent_bg;
	StateMachine.connect("UIPlayerRequest", Callable(self, "_on_ui_player_request"));
	StateMachine.connect("TogglePlayerUI", Callable(self, "_on_toggle_player_ui"));
	StateMachine.connect("PlayerContentTimelineUpdate", Callable(self, "_on_timeline_update"));
	StateMachine.connect("PlayerContentDurationUpdate", Callable(self, "_on_duration_update"));
	transparent_bg.connect("gui_input", Callable(self, "_on_ui_click"));
	transparent_bg.connect("mouse_exited", Callable(self, "_on_ui_mouse_exit"));
	play_pause_button.connect("pressed", Callable(self, "_on_play_pause_button_pressed"));

func _on_toggle_player_ui(value: bool) -> void:
	items_container.visible = value;

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

func _on_timeline_update(value: float) -> void:
	content_timeline.value = value;
	content_elapsed_time.text = format_duration(value);

func format_duration(seconds: float) -> String:
	var total := int(seconds)
	var hours := total / 3600
	var minutes := (total % 3600) / 60
	var secs := total % 60

	if hours > 0:
		return "%02d:%02d:%02d" % [hours, minutes, secs]
	else:
		return "%02d:%02d" % [minutes, secs]

func _on_duration_update(value: int) -> void:
	content_timeline.max_value = value;

func _on_content_timeline_drag_started() -> void:
	StateMachine.ContentSliderUserGrab = true;

func _on_content_timeline_drag_ended(value_changed: bool) -> void:
	StateMachine.set_time_pos(content_timeline.value);
	StateMachine.ContentSliderUserGrab = false;
