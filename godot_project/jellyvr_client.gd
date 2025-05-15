extends Node

func _ready():
	var client = JellyvrClient.new();
	var auth = client.auth();
	
	client.set_server_url("gogole");
