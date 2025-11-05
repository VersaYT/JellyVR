extends Node

class_name UIStateData
var state: UIState
var path: String
var login_username: String

enum UIState {
	SERVER_CONNECT,
	LOGIN,
	LOGIN_WITH_PASSWORD_FROM_PUBLIC_USER,
	LOGIN_WITH_PASSWORD,
	CONTENT,
	SETTINGS,
	ENVIRONMENT
}
