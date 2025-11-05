#extends RigidBody3D
#var mouse_sensivity := 0.005;
#var twist_input := 0.0;
#var pitch_input := 0.0;
#
#@onready var twist_pivot := $TwistPivot
#@onready var pitch_pivot := $TwistPivot/PitchPivot
#
#
## Called when the node enters the scene tree for the first time.
#func _ready() -> void:
	#Input.mouse_mode = Input.MOUSE_MODE_CAPTURED;
#
#
#func _physics_process(delta: float) -> void:
	#var input := Vector3.ZERO
	#input.x = Input.get_axis("left", "right")
	#input.z = Input.get_axis("forward", "back")
	#apply_central_force(twist_pivot.basis * input * 1200.0 * delta)
	#
	#if(Input.is_action_just_pressed("ui_cancel")):
		#Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	#
	#$TwistPivot.rotate_y(-twist_input);
	#$TwistPivot/PitchPivot.rotate_x(-pitch_input)
	##$TwistPivot/PitchPivot.rotation.x = clamp(
		##$TwistPivot/PitchPivot.rotation.x,
			##-0.5,
			##0.5
	##)
	#twist_input = 0.0
	#pitch_input = 0.0
## Called every frame. 'delta' is the elapsed time since the previous frame.
##func _process(delta: float) -> void:
#
#func _unhandled_input(event: InputEvent) -> void:
	#if(event is InputEventMouseMotion):
		#if(Input.mouse_mode == Input.MOUSE_MODE_CAPTURED):
			#twist_input = event.relative.x * mouse_sensivity;
			#pitch_input = event.relative.y * mouse_sensivity
