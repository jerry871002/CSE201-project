#include "obj.h";
//all these numbers are for a house of 100m^2
class LowHouse : public Housing {
public:
	LowHouse();
	~LowHouse();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();

    void simulate_step(double days); // updates every attribute directly as a function of time (days since beginning of the game)
    double insulation, window_cost;
	
	// policies (constructor creates subcritical plant of 38% efficiency) :
	bool solar_panel(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
	bool double_glazing(); // improve insulation
    
};