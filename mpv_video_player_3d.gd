extends Node3D

# Store the MPV player as a class member to prevent garbage collection
var mpv_player: MPVPlayer
var texture_update_count = 0

var play_button: Button
var pause_button: Button
var stop_button: Button

var debug_level = 0;

func _ready():
	
	print("Initializing MPV video player...")
	
	# Create the MPV player instance
	mpv_player = MPVPlayer.new()
	debug_level = mpv_player.get_debug_level();
	
	add_child(mpv_player)
	
	# Initialize the player
	mpv_player.initialize()
	
	# Get the MeshInstance3D node - assuming there's a MeshInstance3D named "Screen" in your scene
	var mesh_instance_3d = $Screen
	
	if mesh_instance_3d:
		print("Found Screen mesh, preparing material...")
		
		# Keep your original mesh and just update its material with the video texture
		# First, make sure it has a material
		var material = mesh_instance_3d.get_surface_override_material(0)
		if not material:
			# Create a new material if none exists
			print("Creating new StandardMaterial3D...")
			material = StandardMaterial3D.new()
			mesh_instance_3d.set_surface_override_material(0, material)
		else:
			print("Using existing material: ", material)
		
		# Make sure the material is visible and properly configured
		if material is StandardMaterial3D:
			material.roughness = 1.0
			material.metallic = 0.0
			material.shading_mode = StandardMaterial3D.SHADING_MODE_UNSHADED
			material.transparency = StandardMaterial3D.TRANSPARENCY_DISABLED
			material.cull_mode = StandardMaterial3D.CULL_DISABLED  # Show both sides
			material.vertex_color_use_as_albedo = false
			print("Material configured for video display")
		
		# Connect to the texture_updated signal to update the material when new frames are available
		print("Connecting to texture_updated signal...")
		mpv_player.connect("texture_updated", _on_texture_updated)
	else:
		print("ERROR: No MeshInstance3D named 'Screen' found in the scene")
	
	# Load and play the video
	print("Loading video file...")
	mpv_player.load_file("/home/versaroumane/Documents/dev/godot/godot_mpv/assets/video/AV1_Big_Buck_Bunny_720_10s.mp4")
	mpv_player.play() 

# This function is called whenever a new video frame is available
func _on_texture_updated(texture):
	texture_update_count += 1
	if debug_level == 2:
		print("Texture updated: ", texture_update_count)
	
	var mesh_instance_3d = $Screen
	if mesh_instance_3d and texture:
		var material = mesh_instance_3d.get_surface_override_material(0)
		if material and material is StandardMaterial3D:
			# Apply the video texture to the albedo texture slot
			material.albedo_texture = texture
			
			# Force material update
			mesh_instance_3d.set_surface_override_material(0, material)
			
			# Optional: Ensure the material settings are optimized for video display
			material.roughness = 1.0  # Non-reflective
			material.metallic = 0.0   # Non-metallic
			material.shading_mode = StandardMaterial3D.SHADING_MODE_UNSHADED  # No lighting effects
			material.transparency = StandardMaterial3D.TRANSPARENCY_DISABLED
			material.cull_mode = StandardMaterial3D.CULL_DISABLED  # Show both sides
			
			if texture_update_count % 10 == 0 && debug_level == 2:  # Log every 10 updates to avoid spam
					print("Applied texture to material")
		else:
			print("ERROR: Material is not a StandardMaterial3D")
	else:
		print("ERROR: Screen mesh not found when updating texture or texture is null")
		
func setup_ui():
	# Create a simple UI for controlling playback (optional)
	var ui_container = VBoxContainer.new()
	ui_container.position = Vector2(20, 20)
	add_child(ui_container)
	
	var button_container = HBoxContainer.new()
	ui_container.add_child(button_container)
	
	# Play button
	play_button = Button.new()
	play_button.text = "Play"
	play_button.pressed.connect(_on_play_button_pressed)
	button_container.add_child(play_button)
	
	# Pause button
	pause_button = Button.new()
	pause_button.text = "Pause"
	pause_button.pressed.connect(_on_pause_button_pressed)
	button_container.add_child(pause_button)
	
	# Stop button
	stop_button = Button.new()
	stop_button.text = "Stop"
	stop_button.pressed.connect(_on_stop_button_pressed)
	button_container.add_child(stop_button)

func _on_play_button_pressed():
	mpv_player.play()

func _on_pause_button_pressed():
	mpv_player.pause()

func _on_stop_button_pressed():
	mpv_player.stop()

func _process(delta):
	# Add a periodic check to ensure things are working
	if Engine.get_frames_drawn() % 60 == 0:  # Check roughly every second
		if texture_update_count == 0:
			print("WARNING: No texture updates received yet!")
			
func _exit_tree():
	# Clean up resources when the scene is exited
	if mpv_player:
		print("Cleaning up MPV player...")
		mpv_player.queue_free()
