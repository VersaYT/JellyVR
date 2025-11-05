extends Control

var spinner_scn = load("res://UI/suspense_spinner.tscn");
var spinner = spinner_scn.instantiate();

@onready var username_input = $HBoxContainer/VBoxContainer/username_container/username_input;
@onready var password_input = $HBoxContainer/VBoxContainer/password_container/password_input;

func _ready() -> void:
	username_input.text = StateMachine.current_state.login_username;


func _on_login_pressed() -> void:
	add_child(spinner);
	if AppManager.auth.login(username_input.text, password_input.text, AppManager.config, AppManager.network) == true:
		var data := UIStateData.new();
		data.state = UIStateData.UIState.CONTENT;
		StateMachine.change_state(data);
		remove_child(spinner);
	
