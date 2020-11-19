#include "obj.h"

class AllPhysicalGoodsFactories: public Production{
public:
	AllPhysicalGoodsFactories();
	~AllPhysicalGoodsFactories();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
};