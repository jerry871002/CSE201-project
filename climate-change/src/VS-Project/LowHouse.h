#include "obj.h"

class LowHouse : public Housing {
public:
	LowHouse();
	~LowHouse();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}