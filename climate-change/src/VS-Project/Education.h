#include "obj.h"

class Education: public Infrastructure {
public:
	Education();
	~Education();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}