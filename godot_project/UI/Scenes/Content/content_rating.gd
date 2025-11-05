extends HBoxContainer

var score: float = 4.5;
@onready var score_label = $"score";

func _ready() -> void:
	score_label.text = str(score);
