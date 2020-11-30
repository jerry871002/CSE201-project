#include "obj.h"
#include "Transport.h"

class Tram: public Transport{
public:
	Tram();
	~Tram();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();
}