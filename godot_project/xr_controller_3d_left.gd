extends XRController3D

@export var idle_seconds := 2.0
@export var movement_threshold := 0.002

var _last_position: Vector3
var _idle_time := 0.0

func _ready():
	_last_position = global_position

func _process(delta):
	var current_position = global_position
	var distance = current_position.distance_to(_last_position)

	if distance < movement_threshold:
		_idle_time += delta
		if _idle_time >= idle_seconds:
			visible = false
	else:
		_idle_time = 0.0
		visible = true

	_last_position = current_position
