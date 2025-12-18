extends Node

class_name UIPlayerState
var play_state: PlayState
var trailer_request: bool
var item: Dictionary

enum PlayState {
	PLAY,
	PAUSE,
	STOP,
	SET_VOLUME,
	RESUME,
	RESTART
}
