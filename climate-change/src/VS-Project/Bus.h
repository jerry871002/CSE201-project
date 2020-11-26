#include "obj.h"

class Bus: public Transport{
public:
	Bus();
	~Bus();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
	void simulate_step(double days);
	double occupancy_rate, capacity;
	double km_perday, fuel_input;
};