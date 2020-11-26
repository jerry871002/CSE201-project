#include "obj.h"

class CoalPowerPlant: public Energy{
public :
	CoalPowerPlant();
	~CoalPowerPlant();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();

	void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	double SO2_output;
	double NOx_output;
	double ash_ouptut;
	double mercury_output;
	double coal;
	double envirnomental_cost;
};
