extends Control


var data : Array = [
	["Year", "Votes", "Else"],
	["2014", 20, 75],
	["2015", 60, 50],
	["2016", 10, 55]
]

# Called when the node enters the scene tree for the first time.
func _ready():
	$LineChart.plot_from_array(data)


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
