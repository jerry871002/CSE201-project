extends Control



# Called when the node enters the scene tree for the first time.
func _ready():
	# Create a timer node
	var timer = Timer.new()

	# Set timer interval
	timer.set_wait_time(10.0)

	# Set it as repeat
	timer.set_one_shot(false)

	# Connect its timeout signal to the function you want to repeat
	timer.connect("timeout", self, "repeat_me")

	# Add to the tree as child of the current node
	add_child(timer)

	timer.start()


func repeat_me():
	#print(get_tree().get_root().get_node("Main/3Dworld").get("statsTotalSatisfaction"))
	#$LineChart.plot_from_array(data)
	$LineChart.plot_from_array(get_tree().get_root().get_node("Main/3Dworld").get("statsTotalSatisfaction"))


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
