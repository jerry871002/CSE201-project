#include "obj.h"

class Mall: public Shop{
public:
	Mall();
	~Mall();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
};