extends Node

class_name UIPlayerState
var play_state: PlayState
var item: Dictionary

enum PlayState {
	PLAY,
	PAUSE,
	RESTART
}
