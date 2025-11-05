extends Label

func _ready() -> void:
	AppManager.auth.connect("error_occurred", _on_connection_error);
func _on_connection_error(message) -> void:
	text = message;
