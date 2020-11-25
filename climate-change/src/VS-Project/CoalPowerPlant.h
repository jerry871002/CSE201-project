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

	void simulate_step(double days);
	double SO2_output;
	double NOx_output;
	double ash_ouptu;
	double mercury_output;
	double coal;

	// policies (constructor creates subcritical plant of 38% efficiency) :
	bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
	bool improvement_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)
};
