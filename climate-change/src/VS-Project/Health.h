#include "obj.h";

class Health: public Infrastructure {
public:
	Health();
	~Health();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}