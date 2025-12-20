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


func _on_audio_track_button_pressed() -> void:
	var data = UIFloatingControlsState.new();
	data.controls_state = UIFloatingControlsState.ControlsState.AUDIO;
	StateMachine.set_floating_controls_active_content(data);
	if StateMachine.current_floating_controls_state != null and StateMachine.current_floating_controls_state.controls_state == UIFloatingControlsState.ControlsState.AUDIO:
		StateMachine.toggle_floating_controls(false);
		StateMachine.current_floating_controls_state = null;
	else:
		StateMachine.toggle_floating_controls(true);
		StateMachine.current_floating_controls_state = data;


func _on_subtitle_track_button_pressed() -> void:
	var data = UIFloatingControlsState.new();
	data.controls_state = UIFloatingControlsState.ControlsState.SUBTITLES;
	StateMachine.set_floating_controls_active_content(data);
	if StateMachine.current_floating_controls_state != null and StateMachine.current_floating_controls_state.controls_state == UIFloatingControlsState.ControlsState.SUBTITLES:
		StateMachine.toggle_floating_controls(false);
		StateMachine.current_floating_controls_state = null;
	else:
		StateMachine.toggle_floating_controls(true);
		StateMachine.current_floating_controls_state = data;


func _on_settings_button_pressed() -> void:
	var data = UIFloatingControlsState.new();
	data.controls_state = UIFloatingControlsState.ControlsState.SETTINGS;
	StateMachine.set_floating_controls_active_content(data);
	if StateMachine.current_floating_controls_state != null and StateMachine.current_floating_controls_state.controls_state == UIFloatingControlsState.ControlsState.SETTINGS:
		StateMachine.toggle_floating_controls(false);
		StateMachine.current_floating_controls_state = null;
	else:
		StateMachine.toggle_floating_controls(true);
		StateMachine.current_floating_controls_state = data;
