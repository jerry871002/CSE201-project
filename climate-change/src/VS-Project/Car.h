#include "obj.h"

class Car: public Transport{
public:
	Car();
	~Car();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}