extends Control

var jelly_api = JellyAPI.new();
var num_of_items := 10;

func _ready() -> void:
	var request = Node.new();
	add_child(request);
	var content_dict = jelly_api.fetch_continue_watch_items(request, num_of_items, AppManager.config, AppManager.network);
	content_dict.connect("request_completed_signal", Callable(self, "_on_content_received"));
	
func _on_content_received(items : Dictionary) -> void:
	for item in items.Items:
		print(item.Name);
