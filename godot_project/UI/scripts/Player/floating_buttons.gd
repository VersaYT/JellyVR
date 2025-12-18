extends Control

func _on_stop_button_pressed() -> void:
	var data = UIPlayerState.new();
	data.play_state = UIPlayerState.PlayState.STOP;
	StateMachine.change_player_ui_state(data);


func _on_volume_button_pressed() -> void:
	var data = UIPlayerState.new();
	data.play_state = UIPlayerState.PlayState.SET_VOLUME;
	StateMachine.change_player_ui_state(data);


func _on_restart_button_pressed() -> void:
	var data = UIPlayerState.new();
	data.play_state = UIPlayerState.PlayState.RESTART;
	StateMachine.change_player_ui_state(data);
