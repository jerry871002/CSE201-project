#include "obj.h";

class HighBuilding : public Housing {
public:
	HighBuilding();
	~HighBuilding();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}