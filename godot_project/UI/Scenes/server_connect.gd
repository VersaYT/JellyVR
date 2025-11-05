extends Control

var spinner_scn = load("res://UI/suspense_spinner.tscn");
var spinner = spinner_scn.instantiate();

func _on_connect_pressed() -> void:
	var input_value = $url_input_field.text;
	spinner.position = Vector2(200, 200);
	add_child(spinner);
	var ping_res = AppManager.auth.ping(input_value);
	if(ping_res) :
		AppManager.network.set_server_url(input_value);
		var data := UIStateData.new();
		data.state = UIStateData.UIState.LOGIN;
		StateMachine.change_state(data);
	
	remove_child(spinner);
	
