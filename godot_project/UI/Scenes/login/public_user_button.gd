extends Button

var username_control;
var profile_pic_control;
var username;

func _on_button_pressed() -> void:
	var data := UIStateData.new();
	data.login_username = username;
	data.state = UIStateData.UIState.LOGIN_WITH_PASSWORD_FROM_PUBLIC_USER;
	StateMachine.change_state(data);

func set_username(user: String) -> void:
	username = user;
	username_control = $container/username;
	username_control.text = user;
	
func set_profile_pic(pic: ImageTexture) -> void:
	profile_pic_control = $container/profile_pic;
	profile_pic_control.texture = pic;
