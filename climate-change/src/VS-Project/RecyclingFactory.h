#include "obj.h"

class RecyclingFactory: public Production{
public:
	RecyclingFactory();
	~RecyclingFactory();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
};