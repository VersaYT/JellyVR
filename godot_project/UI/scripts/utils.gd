extends Node

static func convert_score_10_to_5(score_10: float) -> float:
	# Step 1: Convert score to /5 scale
	var score_5 = score_10 / 2.0
	
	# Step 2: Round up to nearest 0.5
	var rounded = ceil(score_5 * 2.0) / 2.0
	return rounded
