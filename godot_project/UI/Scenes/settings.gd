extends Control

func _ready() -> void:
		var data := NavBarUIStateData.new();
		data.state = NavBarUIStateData.SettingsNavBarState.HOME;
		StateMachine.change_settings_nav_bar_state(data);
