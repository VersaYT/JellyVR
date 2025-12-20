extends Node

@export var current_state: UIStateData
var current_settings_nav_state: NavBarUIStateData
var current_ui_player_state: UIPlayerState
var current_floating_controls_state: UIFloatingControlsState

signal UIChange(data: UIStateData)
signal ActiveBtn(value: String)
signal ActiveSettNavBarBtn(value: String)
signal ToggleUiNavBar(value: bool)
signal UIChangeSettNavBar(data: NavBarUIStateData)
signal UIPlayerRequest(data: UIPlayerState)
signal ToggleUIFloatingButtons(value: bool)
signal SetFloatingControlsActiveContent(data: UIFloatingControlsState);
signal ToggleFloatingControls(value: bool);
signal TogglePlayerUI(value: bool);
signal PlayerVolumeChange(value: float)
signal PlayerContentTimelineUpdate(value: float);
signal PlayerContentDurationUpdate(value: int);
signal PlayerSeekPos(pos: float);
signal PlayerSetAspectRatio(value: String);
signal PlaybackSpeed(speed: String);
signal SetRepeatFile(value: String);

@export var ContentSliderUserGrab : bool = false;
@export var ContentAspectRatio : float;

var ui_state = UIStateData.new();
var nav_ui_state = NavBarUIStateData.new();
var player_ui_state = UIPlayerState.new();

func change_state(data: UIStateData):
	match data.state:
		UIStateData.UIState.SERVER_CONNECT:
			data.path = "res://UI/Scenes/server_connect.tscn"
			current_state = data;
			emit_signal("UIChange", data);
		UIStateData.UIState.LOGIN:
			data.path = "res://UI/Scenes/login.tscn"
			current_state = data;
			emit_signal("UIChange", data);
		UIStateData.UIState.LOGIN_WITH_PASSWORD_FROM_PUBLIC_USER:
			data.path = "res://UI/Scenes/login_with_password.tscn"
			current_state = data;
			emit_signal("UIChange", data);
		UIStateData.UIState.LOGIN_WITH_PASSWORD:
			data.path = "res://UI/Scenes/login_with_password.tscn"
			current_state = data;
			emit_signal("UIChange", data);
		UIStateData.UIState.CONTENT:
			data.path = "res://UI/Scenes/content.tscn"
			current_state = data;
			emit_signal("UIChange", data);
			emit_signal("ActiveBtn", "Content");
		UIStateData.UIState.SETTINGS:
			data.path = "res://UI/Scenes/settings.tscn"
			current_state = data;
			emit_signal("UIChange", data);
			emit_signal("ActiveBtn", "Settings");
		UIStateData.UIState.ENVIRONMENT:
			data.path = "res://UI/Scenes/environment.tscn"
			current_state = data;
			emit_signal("UIChange", data);
			emit_signal("ActiveBtn", "Environment");

func change_settings_nav_bar_state(data: NavBarUIStateData) -> void:
	match data.state:
		NavBarUIStateData.SettingsNavBarState.HOME:
			data.path = "res://UI/Scenes/Settings/home.tscn"
			current_settings_nav_state = data;
			emit_signal("UIChangeSettNavBar", data);
			emit_signal("ActiveSettNavBarBtn", "Home");
		NavBarUIStateData.SettingsNavBarState.CLIENT:
			data.path = "res://UI/Scenes/Settings/client.tscn"
			current_settings_nav_state = data;
			emit_signal("UIChangeSettNavBar", data);
			emit_signal("ActiveSettNavBarBtn", "Client");
			
func toggle_player_ui(value: bool) -> void:
	emit_signal("TogglePlayerUI", value);

func change_player_ui_state(data: UIPlayerState) ->void:
			current_ui_player_state = data;
			emit_signal("UIPlayerRequest", data);

func set_player_volume(value: float) -> void:
	emit_signal("PlayerVolumeChange", value);

func set_time_pos(pos: float) -> void:
	emit_signal("PlayerSeekPos", pos);

func update_timeline(value: float) -> void:
	if ContentSliderUserGrab == false:
		emit_signal("PlayerContentTimelineUpdate", value);
	
func duration_update(value: int) -> void:
	emit_signal("PlayerContentDurationUpdate", value);

func set_player_aspect_ratio(value: String) -> void:
	emit_signal("PlayerSetAspectRatio", value);

func set_playback_speed(speed: String) -> void:
	emit_signal("PlaybackSpeed", speed);

func toggle_floating_controls(value: bool) -> void:
	emit_signal("ToggleFloatingControls", value);

func set_floating_controls_active_content(data: UIFloatingControlsState) -> void:
	emit_signal("SetFloatingControlsActiveContent", data);
	
func set_repeat_file(value: String) -> void:
	emit_signal("SetRepeatFile", value);

func get_content_aspect_ratio() -> float:
	return ContentAspectRatio;
