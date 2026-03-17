extends Control

var jelly_api = JellyAPI.new();
var num_of_items := 10;
var content_card_scn := preload("res://UI/Scenes/Content/ContentCard.tscn");
@onready var items_container := $SmoothScrollContainer/HBoxContainer;

func _ready() -> void:
	var request = Node.new();
	add_child(request);
	var content_dict = jelly_api.fetch_continue_watch_items(request, num_of_items, AppManager.config, AppManager.network);
	content_dict.connect("request_completed_signal", Callable(self, "_on_content_received"));
	
func _on_content_received(content : Dictionary) -> void:
	
	for item in content["Items"]:
		var content_card = content_card_scn.instantiate();
		content_card.title = item["Name"];
		content_card.item_ref = item;
		content_card.progress = int(item["UserData"]["PlayedPercentage"]);
		match item["Type"]:
			"Episode":
				content_card.sub_info = "S" + str(int(item["ParentIndexNumber"]))  + " - EP " + str(int(item["IndexNumber"]));
			"Movie":
				content_card.sub_info = str(int(item["ProductionYear"]));
		content_card.item_id = item["ParentThumbItemId"];
		items_container.add_child(content_card);
