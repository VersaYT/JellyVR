extends Control

var jellyfin_api = JellyAPI.new();
signal CollectionFolderIdSet;

func _ready() -> void:
	if !AppManager.config.get_collection_folders():
		var request = Node.new();
		add_child(request);
		var collection_folders_list = jellyfin_api.get_collection_folders(request, AppManager.config, AppManager.network);
		collection_folders_list.connect("request_completed_signal", Callable(self, "_on_collection_folders_received"));


func _on_collection_folders_received(folders_collection) -> void:
	AppManager.config.set_collection_folders(folders_collection["Items"]);
	if AppManager.config.settings.home.get_ActiveCarouselCollectionFolderId() == "":
		var keys = ["Settings", "Home", "ActiveCarouselCollectionFolderId"];
		AppManager.config.set_config_value(keys, folders_collection["Items"][0]["Id"]);
		AppManager.config.settings.home.set_ActiveCarouselCollectionFolderId(folders_collection["Items"][0]["Id"]);
		emit_signal("CollectionFolderIdSet");
