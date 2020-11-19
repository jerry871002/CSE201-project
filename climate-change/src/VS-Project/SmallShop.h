#include "obj.h"
class SmallShop: public Shop{
public:
	SmallShop();
	~SmallShop();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
};