extends Node

func _ready():
	var client = JellyvrClient.new();
	var auth = client.auth();
	var config = AppConfig.new();
	
	client.set_server_url("url here");
	auth.login("test", "test", config);
