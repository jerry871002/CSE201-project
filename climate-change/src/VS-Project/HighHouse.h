#include "obj.h";

class HighHouse : public Housing {
public:
	HighHouse();
	~HighHouse();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}