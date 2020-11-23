#include "obj.h"

//very old kinda living in a barn
class LowHouse : public Housing {
public:
	LowHouse();
	~LowHouse();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();

 
    //attributes:
    double efficiency;
    //ake attrubutes from inherited class
    void simulate_step(double t) {
    double insulation;
    //fenêtre quels types de mur comment est fait le heating elctricité gaz
    //low satisfaction 
    //simple vitrage
    // ventilation, would actually come from those poorly insulated walls and windows
    //probalby can say no solar panels 
    }
}