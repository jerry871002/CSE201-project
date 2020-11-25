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
	protected:
	double capacity, occupancy_rate;
};