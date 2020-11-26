#include "obj.h"

class NuclearPowerPlant: public Energy{
public:
	NuclearPowerPlant();
	~NuclearPowerPlant();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();

	void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	double mercury_output;
	double envirnomental_cost;
};