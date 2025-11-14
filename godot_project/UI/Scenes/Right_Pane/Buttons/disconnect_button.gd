extends Button

func _on_pressed() -> void:
	StateMachine.connect("UIChange", Callable(self, "_on_ui_change"));
	var data := UIStateData.new();
	data.state = UIStateData.UIState.LOGIN;
	StateMachine.change_state(data)
	StateMachine.emit_signal("ToggleUiNavBar", false);
	AppManager.auth.logout(AppManager.config);
