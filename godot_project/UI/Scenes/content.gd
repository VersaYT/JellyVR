extends Control

var jellyfin_api = JellyAPI.new();

func _ready() -> void:
	var request = Node.new();
	add_child(request);
	var collection_folders_list = jellyfin_api.get_collection_folders(request, AppManager.config, AppManager.network);
	collection_folders_list.connect("request_completed_signal", Callable(self, "_on_collection_folders_received"));

func _on_collection_folders_received(folders_collection) -> void:
	AppManager.config.set_collection_folders(folders_collection["Items"]);
