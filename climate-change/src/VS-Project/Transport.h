#include "obj.h"

class Transport : public Infrastructure {
public:
    double numCars;
    double numElectricCars;
    double numBus;
    double numTram;
    double occupancyRate, capacity, kmPerDay,passengers, co2PerKm, fuelPerKm; // co2 in kg
    double fuelInput; // in litres
    int type; /*
    0 - electic car, 1 - big american car 2 - normal car 
    3 - old collection car 4 - bike 5 -  motorcycle 6 - bus*/
    Transport(int transportType);
	~Transport();
	/*void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();*/
	void simulate_step(double days);
};