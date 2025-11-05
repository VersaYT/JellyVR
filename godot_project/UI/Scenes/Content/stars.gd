extends HBoxContainer

var full_star_icon = preload("res://UI/assets/img/Content/carousel/star_full.png");
var half_star_icon = preload("res://UI/assets/img/Content/carousel/star_half.png");
var empty_star_icon = preload("res://UI/assets/img/Content/carousel/star_empty.png");

func _ready() -> void:
	match get_parent().score:
		0.5:
			var half_star = TextureRect.new();
			half_star.texture = half_star_icon;
			half_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
			half_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
			add_child(half_star);
			for i in range(4):
				var empty_star = TextureRect.new();
				empty_star.texture = empty_star_icon;
				empty_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				empty_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(empty_star);
		1:
			var full_star = TextureRect.new();
			full_star.texture = full_star_icon;
			full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
			full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
			add_child(full_star);
			for i in range(4):
				var empty_star = TextureRect.new();
				empty_star.texture = empty_star_icon;
				empty_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				empty_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(empty_star);
		1.5:
			var full_star = TextureRect.new();
			full_star.texture = full_star_icon;
			full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
			full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
			add_child(full_star);
			
			var half_star = TextureRect.new();
			half_star.texture = half_star_icon;
			half_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
			half_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
			add_child(half_star);
			
			for i in range(3):
				var empty_star = TextureRect.new();
				empty_star.texture = empty_star_icon;
				empty_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				empty_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(empty_star);
		2:
			for i in range(2):
				var full_star = TextureRect.new();
				full_star.texture = full_star_icon;
				full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(full_star);
			
			for i in range(3):
				var empty_star = TextureRect.new();
				empty_star.texture = empty_star_icon;
				empty_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				empty_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(empty_star);
		2.5:
			for i in range(2):
				var full_star = TextureRect.new();
				full_star.texture = full_star_icon;
				full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(full_star);

			var half_star = TextureRect.new();
			half_star.texture = half_star_icon;
			half_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
			half_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
			add_child(half_star);
			
			for i in range(2):
				var empty_star = TextureRect.new();
				empty_star.texture = empty_star_icon;
				empty_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				empty_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(empty_star);
		3:
			for i in range(3):
				var full_star = TextureRect.new();
				full_star.texture = full_star_icon;
				full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(full_star);
			
			for i in range(2):
				var empty_star = TextureRect.new();
				empty_star.texture = empty_star_icon;
				empty_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				empty_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(empty_star);
		3.5:
			for i in range(3):
				var full_star = TextureRect.new();
				full_star.texture = full_star_icon;
				full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(full_star);

			var half_star = TextureRect.new();
			half_star.texture = half_star_icon;
			half_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
			half_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
			add_child(half_star);
			
			for i in range(1):
				var empty_star = TextureRect.new();
				empty_star.texture = empty_star_icon;
				empty_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				empty_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(empty_star);
		4:
			for i in range(4):
				var full_star = TextureRect.new();
				full_star.texture = full_star_icon;
				full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(full_star);
			
			for i in range(1):
				var empty_star = TextureRect.new();
				empty_star.texture = empty_star_icon;
				empty_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				empty_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(empty_star);
		4.5:
			for i in range(4):
				var full_star = TextureRect.new();
				full_star.texture = full_star_icon;
				full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(full_star);

			var half_star = TextureRect.new();
			half_star.texture = half_star_icon;
			half_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
			half_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
			add_child(half_star);
		5:
			for i in range(5):
				var full_star = TextureRect.new();
				full_star.texture = full_star_icon;
				full_star.expand_mode = TextureRect.EXPAND_FIT_WIDTH;
				full_star.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT;
				add_child(full_star);
