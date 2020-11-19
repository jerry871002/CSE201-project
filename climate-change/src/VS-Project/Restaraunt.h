#include "obj.h"
class Restaraunt: public Shop{
public:
	Restaraunt();
	~Restaraunt();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
};