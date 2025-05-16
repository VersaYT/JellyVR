extends Node

func _ready():
	var client = JellyvrClient.new();
	var auth = client.auth();
	
	client.set_server_url("http://192.168.1.118:30013");
	auth.login("Admin", "23755804");
