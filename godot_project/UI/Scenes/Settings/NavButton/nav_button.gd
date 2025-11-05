extends Button

@onready var btn: Button = self;

func _ready() -> void:
	StateMachine.connect("ActiveSettNavBarBtn", Callable(self, "_on_received_button_state"));

func _on_button_pressed() -> void:
	match btn.text:
		"Home":
			var data := NavBarUIStateData.new();
			data.state = NavBarUIStateData.SettingsNavBarState.HOME;
			StateMachine.change_settings_nav_bar_state(data);
		"Client":
			var data := NavBarUIStateData.new();
			data.state = NavBarUIStateData.SettingsNavBarState.CLIENT;
			StateMachine.change_settings_nav_bar_state(data);


func _on_received_button_state(value: String) -> void:
	if value == btn.text:
		var active_style = theme.get_stylebox("active", "Button");
		add_theme_stylebox_override("normal", active_style);
		var active_style_hover = theme.get_stylebox("active_hover", "Button");
		add_theme_stylebox_override("hover", active_style_hover);
		var active_style_click = theme.get_stylebox("active_click", "Button");
		add_theme_stylebox_override("pressed", active_style_click);
	else:
		remove_theme_stylebox_override("normal");
		remove_theme_stylebox_override("hover");
		remove_theme_stylebox_override("pressed");
