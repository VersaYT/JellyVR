extends HBoxContainer

var buttons = ["Home", "Client"];

var btn_node = load("res://UI/Scenes/Settings/NavButton/nav_button.tscn");

func _ready() -> void:
	for button in buttons:
		var btn: Button = btn_node.instantiate();
		btn.text = button;
		add_child(btn);
