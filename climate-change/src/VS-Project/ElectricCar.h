#include "obj.h"

class ElectricCar: public Transport{
public:
	ElectricCar();
	~ElectricCar();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}