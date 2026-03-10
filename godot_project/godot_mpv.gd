extends Node3D

# Store the MPV player as a class member to prevent garbage collection
var mpv_player: MPVPlayer;
var texture_update_count = 0

var debug_level = 0010;

@export var content_item: Dictionary;
@export var trailer_request: bool;
@onready var mesh_instance_3d = get_parent().get_node("Screen_Container/Screen");
@onready var material = mesh_instance_3d.get_surface_override_material(0);
@onready var headers_dict;
@onready var full_yt_dlp_path;
func _ready():
	StateMachine.connect("PlayerVolumeChange", Callable(self, "set_player_volume"));
	StateMachine.connect("PlayerSeekPos", Callable(self, "seek_content_pos"));
	StateMachine.connect("PlayerSetAspectRatio", Callable(self, "set_aspect_ratio"));
	StateMachine.connect("PlaybackSpeed", Callable(self, "set_playback_speed"));
	StateMachine.connect("SetRepeatFile", Callable(self, "set_repeat_file"));
	print("Initializing MPV video player...")
	
	# Create the MPV player instance
	mpv_player = MPVPlayer.new()
	mpv_player.set_debug_level(debug_level);
	mpv_player.connect("time_changed", Callable(self, "_timeline_update"));
	add_child(mpv_player)
	# Initialize the player
	mpv_player.initialize()
		# Connect to the texture_updated signal to update the material when new frames are available
	print("Connecting to texture_updated signal...")
	mpv_player.connect("texture_updated", _on_texture_updated)


func play() -> void:
	
	if mesh_instance_3d:
		print("Found Screen mesh, preparing material...")
	
	# Keep your original mesh and just update its material with the video texture
	# First, make sure it has a material
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
	var url;
	if not trailer_request:
		headers_dict = {
			"Authorization": "MediaBrowser Client=\"JellyVR\", Version=\"" + AppManager.config.get_app_version() + ", DeviceId=\"" + AppManager.config.get_device_id() + ", Device=\"JellyVR Client\", Token=\"" + AppManager.config.get_access_token() + "\""
		}
		print(headers_dict_to_lavf(headers_dict));
		url = AppManager.network.get_server_url() + "/Videos/" + content_item["Id"] + "/stream?static=true&mediaSourceId=" + content_item["MediaSources"][0]["Id"] + "&tag=" + content_item["Etag"] + "&directPlay=true";
	var user_path = ProjectSettings.globalize_path("user://")
	full_yt_dlp_path = user_path + "/Bin/yt-dlp/" + AppManager.config.yt_dlp_binary_name;
	# Load and play the video
	print("Loading video file...")
	if not trailer_request:
		mpv_player.load_file(url, headers_dict_to_lavf(headers_dict), full_yt_dlp_path);
	else:
		mpv_player.load_file(content_item["RemoteTrailers"][0]["Url"], "", full_yt_dlp_path);
	mpv_player.play()
	StateMachine.ContentAspectRatio = mpv_player.get_content_aspect_ratio();

func resume() -> void:
	mpv_player.play();

func pause() -> void:
	mpv_player.pause();
	
func restart() -> void:
	mpv_player.restart();
	
func seek_content_pos(pos: float) -> void:
	mpv_player.seek_content_pos(str(pos));

func set_player_volume(value: float) -> void:
	mpv_player.set_volume(str(value));

func _timeline_update(time: float) -> void:
	StateMachine.update_timeline(time);

func set_aspect_ratio(value: String) -> void:
	mpv_player.set_aspect_ratio(value);

func set_playback_speed(speed: String) -> void:
	mpv_player.set_playback_speed(speed);

func set_repeat_file(value: String) -> void:
	mpv_player.set_repeat_file(value);

func stop() -> void:
	mpv_player.stop();
	await get_tree().create_timer(1.0).timeout
	var material = mesh_instance_3d.get_surface_override_material(0);
	var texture = load("res://UI/assets/img/virtual_screen_no_content_bg.jpg");
	material.albedo_texture = texture;
	mesh_instance_3d.set_surface_override_material(0, material);
# This function is called whenever a new video frame is available
func _on_texture_updated(texture):
	texture_update_count += 1
	if debug_level == 2:
		print("Texture updated: ", texture_update_count)

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

func headers_dict_to_lavf(headers: Dictionary):
	var result = ""
	for key in headers.keys():
		var value = headers[key]
		result += str(key) + ": " + str(value) + "\r\n"
		return result
