extends Node

@export var current_state: UIStateData
var current_settings_nav_state: NavBarUIStateData
var current_ui_player_state: UIPlayerState

signal UIChange(data: UIStateData)
signal ActiveBtn(value: String)
signal ActiveSettNavBarBtn(value: String)
signal ToggleUiNavBar(value: bool)
signal UIChangeSettNavBar(data: NavBarUIStateData)
signal UIPlayerRequest(data: UIPlayerState)

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
			
func change_player_ui_state(data: UIPlayerState) ->void:
			current_ui_player_state = data;
			emit_signal("UIPlayerRequest", data);
