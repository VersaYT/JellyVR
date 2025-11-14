extends Node

@onready var logo = $Logo;

func _ready() -> void:
	StateMachine.connect("UIChange", Callable(self, "_on_ui_change"));
	
	var data := UIStateData.new();

	if AppManager.network.get_server_url() == "":
		data.state = UIStateData.UIState.SERVER_CONNECT;
		#StateMachine.change_state(data);
		#_load_ui(data);
	elif not AppManager.auth.get_is_logged_in():
		data.state = UIStateData.UIState.LOGIN;
		#StateMachine.change_state(data);
		#_load_ui(data);
	else:
		data.state = UIStateData.UIState.CONTENT;
		#StateMachine.change_state(data);
		#_load_ui(data);
	
	StateMachine.change_state(data)
		
func _on_ui_change(data: UIStateData) -> void:
	_load_ui(data);
	if data.state == UIStateData.UIState.LOGIN || data.state == UIStateData.UIState.LOGIN_WITH_PASSWORD_FROM_PUBLIC_USER || data.state == UIStateData.UIState.LOGIN_WITH_PASSWORD:
		logo.visible = true;
	else:
		logo.visible = false;

func _load_ui(data):
	if $ui_panel.get_child_count() > 0:
		$ui_panel.get_child(0).queue_free()

	var ui = load(data.path).instantiate()
	$ui_panel.add_child(ui)
