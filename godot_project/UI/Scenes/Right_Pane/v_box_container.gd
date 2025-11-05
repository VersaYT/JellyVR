extends VBoxContainer

var buttons = [
	{"label": "Content", "icon": "logo_icon"},
	{"label": "Environment", "icon": "environment_icon"},
	{"label": "Settings", "icon": "settings_icon"}
]

var btn_node = load("res://UI/Scenes/Right_Pane/Buttons/button.tscn");
var disconnect_btn = load("res://UI/Scenes/Right_Pane/Buttons/disconnect_button.tscn");
var disconnect_btn_node = disconnect_btn.instantiate();

func _ready() -> void:
	for button in buttons:
		var btn: Button = btn_node.instantiate();
		var label = btn.get_node("HBoxContainer/VBoxContainer/label");
		var icon = btn.get_node("HBoxContainer/VBoxContainer/icon");
		label.text = button.label;
		icon.texture = load("res://UI/assets/img/right_pane_buttons/" + button.icon + ".png");
		add_child(btn);
	add_child(disconnect_btn_node);
