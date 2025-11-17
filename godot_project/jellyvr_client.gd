extends Node

var xr_interface = XRInterface;
@onready var UI : Node3D = $UI;
@onready var navbar: Node3D = $UI/Right_Pane;
var XRCamera: XRCamera3D;
var ui_offset_distance = 0.7;
var ui_offset := Vector3(0,0,-ui_offset_distance);
func _ready():
	xr_interface = XRServer.find_interface("OpenXR")
	if xr_interface and xr_interface.is_initialized():
		DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_DISABLED)
	get_viewport().use_xr = true;
	var XRControllerRight : XRController3D = $XROrigin3D/XRController3D_right;
	var XRControllerLeft : XRController3D = $XROrigin3D/XRController3D_left;
	XRControllerLeft.connect("button_pressed", Callable(self, "_on_button_pressed"))
	StateMachine.connect("UIPlayerRequest", Callable(self, "_on_player_request"));
	StateMachine.connect("ToggleUiNavBar", Callable(self, "on_toggle_ui_navbar"));
	var data := UIStateData.new();
	data.state = UIStateData.UIState.CONTENT;
	print(StateMachine.current_state.state)
	if StateMachine.current_state.state == data.state:
		on_toggle_ui_navbar(true);
	XRCamera = $XROrigin3D/XRCamera3D;

func _on_player_request(data):
	var godot_mpv = load("res://godot_mpv.gd");
	var mpv = godot_mpv.new();
	mpv.trailer_request = data.trailer_request;
	mpv.content_item = data.item;
	add_child(mpv);
	
func _on_button_pressed(name):
	match name:
		"menu_button":
			open_close_menu()
	
func open_close_menu():
	if UI.visible == false:
		var ui_global_transform := Transform3D(XRCamera.global_basis, XRCamera.global_position)
		ui_global_transform.origin += ui_global_transform.basis * ui_offset
		UI.global_transform = ui_global_transform;
		UI.visible = true;
	else:
		UI.visible = false;

func on_toggle_ui_navbar(value: bool):
	navbar.visible = value;
