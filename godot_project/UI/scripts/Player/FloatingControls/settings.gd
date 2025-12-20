extends Control

@onready var normal_btn := $aspect_ratio/default;

func _ready() -> void:
	StateMachine.connect("PlayerSetAspectRatio", Callable(self, "_on_aspect_ratio"));

func _on_default_pressed() -> void:
	StateMachine.set_player_aspect_ratio("no");


func _on_16_9_pressed() -> void:
	StateMachine.set_player_aspect_ratio("16:9");


func _on_4_3_pressed() -> void:
	StateMachine.set_player_aspect_ratio("4:3");

func _on_normal_pressed() -> void:
	StateMachine.set_player_aspect_ratio(str(StateMachine.ContentAspectRatio));

func _on_1x_pressed() -> void:
	StateMachine.set_playback_speed("1.0");

func _on_2x_pressed() -> void:
	StateMachine.set_playback_speed("2.0");

func _on_3x_pressed() -> void:
	StateMachine.set_playback_speed("3.0");

func _on_none_pressed() -> void:
	StateMachine.set_repeat_file("no");
	
func _on_repeat_all_pressed() -> void:
	StateMachine.set_repeat_file("inf");

func _on_repeat_once_pressed() -> void:
	StateMachine.set_repeat_file("1");
