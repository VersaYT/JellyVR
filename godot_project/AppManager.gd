extends Node
var client : JellyvrClient
var auth : Auth
var config : AppConfig
var network : NetworkConfig

func _ready() -> void:
	client = JellyvrClient.new()
	auth = client.get_Auth()
	auth.connect("HTTPSRedirect", Callable(self, "_on_https_redirect"));
	config = client.get_AppConfig()
	config.init();
	client.populate_settings_from_config_file();
	network = client.get_NetworkConfig()

func _on_https_redirect() -> void:
	StateMachine.HTTPSRedirect = true;
