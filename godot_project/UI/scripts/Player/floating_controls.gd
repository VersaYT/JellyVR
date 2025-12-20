extends Control

@onready var title := $title;
@onready var content_container := $content_container;
var settings_scene = preload("res://UI/Scenes/Player/FloatingControlsPages/Settings.tscn");

func _ready() -> void:
	StateMachine.connect("SetFloatingControlsActiveContent", Callable(self, "_on_set_floating_controls_active_content"));
	
func _on_set_floating_controls_active_content(data: UIFloatingControlsState) -> void:
	match data.controls_state:
		UIFloatingControlsState.ControlsState.SETTINGS:
			for child in content_container.get_children():
				content_container.remove_child(child);
			title.text = "Settings";
			content_container.add_child(settings_scene.instantiate());
		UIFloatingControlsState.ControlsState.AUDIO:
			for child in content_container.get_children():
				content_container.remove_child(child);
			title.text = "Audio Tracks";
		UIFloatingControlsState.ControlsState.SUBTITLES:
			for child in content_container.get_children():
				content_container.remove_child(child);
			title.text = "Subtitle Tracks";
