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
};