extends Control

@onready var user_card_button := $user_card_button;

func _ready() -> void:
	user_card_button.text = AppManager.config.get_username();
	var request = Node.new();
	add_child(request);
	var user_profile_pic_request = AppManager.auth.get_user_profile_pic(request, AppManager.config.get_user_id(), AppManager.network);
	user_profile_pic_request.connect("received_image", Callable(self, "_on_received_profile_pic"));

func _on_received_profile_pic(image: ImageTexture) -> void:
	user_card_button.icon = image;
 
