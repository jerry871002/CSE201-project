#include "Bus.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include "random"
using namespace godot;

void Bus::_register_methods()
{
	register_method((char*)"_process", &Bus::_process);
	register_method((char*)"_input", &Bus::_input);
	register_method((char*)"_ready", &Bus::_ready);
}

void Bus::_init()
{

}

void Bus::_process(float delta)
{

}

void Bus::_input(InputEvent* e)
{

}

void Bus::_ready()
{

}


Bus::Bus()
{
// random device class instance, source of 'true' randomness for initializing random seed
std::random_device rd; 
// Mersenne twister PRNG, initialized with seed from previous random device instance
std::mt19937 gen(rd()); 
std::normal_distribution <double> d(262500, 52500);
cost = d(gen); // cost of 1 bus in euros, randomised using gaussian
double alpha = (cost-262500)/262500;
if (alpha<0) alpha=0;
if (alpha > 1) alpha =1;
capacity = 30+ alpha*30;  
capacity  = round(capacity); //maximum number of people in 1 bus, positive correlation with cost 
std::normal_distribution <double> d(0.70, 0.30);
occupancy_rate = d(gen); // average percentage occupancy of the bus: number of people in the bus / capacity
if (occupancy_rate>1) occupancy_rate=1;
std::normal_distribution <double> d(22, 3);
building_time = d(gen); // construction time for 1 bus, in days
std::normal_distribution <double> d(8, 1.5); // satisfaction level rather high, randomised
satisfaction = d(gen);
if (satisfaction >10) satisfaction = 10;
std::normal_distribution <double> d(225, 75);
km_perday = d(gen); // kilometres per day,  randomised for each bus
std::normal_distribution <int> d(3, 1);
employment = d(gen);
if (employment <1) employment =1;
CO2_output = 0; // co2 output for the whole duration of simulation
maintenance = 0; // maintenance cost for the whole duration of simulation
fuel_input = 0; // fuel needed for the whole duration of simulation
passengers = 0; //total number of passengers that used the bus
}

void Bus::simulate_step(double days)
{
fuel_input+= 26/100*km_perday*days; //litres of fuel for bus
double alpha = (cost-262500)/262500;
if (alpha<0) alpha=0;
if (alpha > 1) alpha =1;
maintenance += (0.67+alpha*0.17)*km_perday; // maintenance cost in euros
CO2_output+=1250/1000*occupancy_rate*capacity*km_perday*days; //co2 output in kg 
passengers+=capacity*occupancy_rate*days; //number of people that used the bus in given period
}

Bus::~Bus()
{

}