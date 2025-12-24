extends Node

var xr_interface = XRInterface;
@onready var UI : Node3D = $UI;
@onready var Keyboard : Node3D = $UI/Keyboard;
@onready var navbar: Node3D = $UI/Right_Pane;
@onready var left_navbar: Node3D = $UI/Left_Pane;
@onready var floating_controls := $PlayerUI_Floating_Controls;
@onready var floating_buttons := $PlayerUI_Floating_Buttons;
@onready var floating_volume_slider = $PlayerUI_Floating_Volume_Slider;
var XRControllerLeft : XRController3D;
var XRControllerRight : XRController3D;
var XRCamera: XRCamera3D;
var ui_offset_distance = 0.7;
var ui_offset := Vector3(0,0,-ui_offset_distance);

var godot_mpv = load("res://godot_mpv.gd");
@onready var mpv = godot_mpv.new();
func _ready():
	xr_interface = XRServer.find_interface("OpenXR")
	if xr_interface and xr_interface.is_initialized():
		DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_DISABLED)
	get_viewport().use_xr = true;
	XRControllerRight = $XROrigin3D/XRController3D_right;
	XRControllerLeft = $XROrigin3D/XRController3D_left;
	XRControllerLeft.show_when_tracked = true;
	XRControllerRight.show_when_tracked = true;
	XRControllerLeft.connect("button_pressed", Callable(self, "_on_button_pressed"));
	XRControllerRight.connect("button_pressed", Callable(self, "_on_button_pressed"));
	StateMachine.connect("UIPlayerRequest", Callable(self, "_on_player_request"));
	StateMachine.connect("ToggleUiNavBar", Callable(self, "on_toggle_ui_navbar"));
	StateMachine.connect("TogglePlayerUI", Callable(self, "_on_toggle_player_ui"));
	StateMachine.connect("ToggleUiLeftNavBar", Callable(self, "on_toggle_ui_left_navbar"));
	StateMachine.connect("ToggleUIFloatingButtons", Callable(self, "_on_toggle_ui_floating_buttons"));
	StateMachine.connect("ToggleFloatingControls", Callable(self, "_on_toggle_floating_controls"));
	add_child(mpv);
	var data := UIStateData.new();
	data.state = UIStateData.UIState.CONTENT;
	print(StateMachine.current_state.state)
	if StateMachine.current_state.state == data.state:
		on_toggle_ui_navbar(true);
		on_toggle_ui_left_navbar(true);
	XRCamera = $XROrigin3D/XRCamera3D;

func _on_toggle_player_ui(value: bool) -> void:
	if value == false:
		floating_buttons.visible = value;
		floating_controls.visible = value;
		floating_volume_slider.visible = value;
	else:
		floating_buttons.visible = value;

func _on_player_request(data):
	match data.play_state:
		UIPlayerState.PlayState.PLAY:
			mpv.trailer_request = data.trailer_request;
			mpv.content_item = data.item;
			mpv.play();
		UIPlayerState.PlayState.PAUSE:
			mpv.pause();
		UIPlayerState.PlayState.RESTART:
			mpv.restart();
		UIPlayerState.PlayState.SET_VOLUME:
			floating_volume_slider.visible = !floating_volume_slider.visible;
		UIPlayerState.PlayState.RESUME:
			mpv.resume();
			floating_volume_slider.visible = false;
		UIPlayerState.PlayState.STOP:
			mpv.stop();
			floating_volume_slider.visible = false;
			mpv.queue_free()


func _on_button_pressed(name):
	match name:
		"trigger_click":
			var controller_ray := $XROrigin3D/XRController3D_right/FunctionPointer/RayCast;
			var collider = controller_ray.get_collider();
			if collider == null:
				StateMachine.toggle_player_ui(false);
		"menu_button":
			open_close_menu()
		"by_button":
			open_close_keyboard()

func _on_toggle_ui_floating_buttons(value: bool):
	var ui_floating_buttons = $PlayerUI_Floating_Buttons;
	ui_floating_buttons.visible = value;

func open_close_keyboard():
	if Keyboard.visible == false and UI.visible == true:
		Keyboard.visible = true;
	else:
		Keyboard.visible = false;

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

func on_toggle_ui_left_navbar(value: bool):
	left_navbar.visible = value;

func _on_toggle_floating_controls(value: bool) -> void:
	floating_controls.visible = value;
