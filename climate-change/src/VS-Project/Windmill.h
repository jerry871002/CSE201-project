#include "obj.h"

class Windmill: public Energy{
public:
	Windmill();
	~Windmill();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}
