extends Node

var xr_interface = XRInterface;
func _ready():
	xr_interface = XRServer.find_interface("OpenXR")
	
	if xr_interface and xr_interface.is_initialized():
		DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_DISABLED)
		
	get_viewport().use_xr = true;
	
	StateMachine.connect("UIPlayerRequest", Callable(self, "_on_player_request"));

func _on_player_request(data):
	var godot_mpv = load("res://godot_mpv.gd");
	var mpv = godot_mpv.new();
	mpv.content_item = data.item;
	add_child(mpv);
	
