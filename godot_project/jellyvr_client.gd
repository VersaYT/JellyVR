extends Node

func _ready():
	var client = JellyvrClient.new();
	var auth = client.auth();
	
	var result = auth._ping("http://192.168.1.118:30013");
