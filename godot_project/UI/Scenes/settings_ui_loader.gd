extends Control

func _ready() -> void:
	StateMachine.connect("UIChangeSettNavBar", Callable(self, "_on_settings_navbar_change"));

func _on_settings_navbar_change(data: NavBarUIStateData) -> void:
	_load_settings_ui(data);
	
func _load_settings_ui(data):
	if $".".get_child_count() > 0:
		$".".get_child(0).queue_free()

	var ui = load(data.path).instantiate()
	$".".add_child(ui)
