#include "obj.h"

class Services: public Production{
public:
	Services();
	~Services();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
};