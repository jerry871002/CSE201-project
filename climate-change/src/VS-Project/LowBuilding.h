#include "obj.h"

class LowBuilding : public Housing {
public:
	LowBuilding();
	~LowBuilding();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}