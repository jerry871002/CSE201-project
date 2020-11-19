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
};