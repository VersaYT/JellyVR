extends Node
var client : JellyvrClient
var auth : Auth
var config : AppConfig
var network : NetworkConfig

func _ready() -> void:
	client = JellyvrClient.new()
	auth = client.get_Auth()
	config = client.get_AppConfig()
	network = client.get_NetworkConfig()
