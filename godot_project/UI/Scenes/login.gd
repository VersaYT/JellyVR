extends Control

@onready var users_container: HBoxContainer = $public_users_container;

func _ready() -> void:
	var request = Node.new();
	add_child(request);
	var users_list_request = AppManager.auth.get_public_users(request, AppManager.network);
	users_list_request.connect("request_completed_signal", Callable(self, "_on_users_received"));

func _on_received_profile_pic(image: ImageTexture, user: Dictionary, btn_ref: Button) -> void:
	var profile_pic: TextureRect = btn_ref.get_node("container/profile_pic");
	profile_pic.texture = image;

func _on_users_received(users):
	var public_user_btn_scn = preload("res://UI/Scenes/login/public_user_button.tscn");
	
	for user in users:
		var request = Node.new();
		add_child(request);
		var user_btn = public_user_btn_scn.instantiate();
		user_btn.set_username(user.Name);
		var user_profile_pic_request = AppManager.auth.get_user_profile_pic(request, user.Id, AppManager.network);
		user_profile_pic_request.connect("received_image", Callable(self, "_on_received_profile_pic").bind(user, user_btn));
		users_container.add_child(user_btn);


func _on_connect_with_password_pressed() -> void:
	var data := UIStateData.new();
	data.state = UIStateData.UIState.LOGIN_WITH_PASSWORD;
	StateMachine.change_state(data)


func _on_change_server_pressed() -> void:
	var data := UIStateData.new();
	data.state = UIStateData.UIState.SERVER_CONNECT;
	StateMachine.change_state(data);
	StateMachine.HTTPSRedirect = false;
	AppManager.client.clean_server_related_config();
