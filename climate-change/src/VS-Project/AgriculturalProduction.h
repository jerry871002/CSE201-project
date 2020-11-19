#include "obj.h"

class AgriculturalProduction: public Services{
public:
	AgriculturalProduction();
	~AgriculturalProduction();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
};