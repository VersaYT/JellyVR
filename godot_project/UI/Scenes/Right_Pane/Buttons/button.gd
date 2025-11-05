extends Button

@onready var label = $HBoxContainer/VBoxContainer/label;

func _ready() -> void:
	StateMachine.connect("ActiveBtn", Callable(self, "_on_received_button_state"));
	
func _on_button_pressed() -> void:
	match label.text:
		"Content":
			var data := UIStateData.new();
			data.state = UIStateData.UIState.CONTENT;
			StateMachine.change_state(data);
		"Environment":
			var data := UIStateData.new();
			data.state = UIStateData.UIState.ENVIRONMENT;
			StateMachine.change_state(data);
		"Settings":
			var data := UIStateData.new();
			data.state = UIStateData.UIState.SETTINGS;
			StateMachine.change_state(data);


func _on_received_button_state(value: String) -> void:
	if value == label.text:
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
