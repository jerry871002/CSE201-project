extends Control


var data : Array = [
	["Country", "Votes"],
	["USA", 20],
	["UK", 55],
	["FRA", 10],
	["GER", 15]
]

# Called wen the node enters the scene tree for the first time.
func _ready():
	$PieChart.plot_from_array(data)

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
