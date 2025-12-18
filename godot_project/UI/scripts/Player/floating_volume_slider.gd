extends Control

@onready var volume_slider := $volume_slider;

func _ready() -> void:
	volume_slider.connect("value_changed", Callable(self, "_on_value_changed"));
	
func _on_value_changed(value: float) -> void:
	StateMachine.set_player_volume(value);
