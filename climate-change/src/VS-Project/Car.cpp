#include "Car.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include "random"
using namespace godot;

void Car::_register_methods()
{
	register_method((char*)"_process", &Car::_process);
	register_method((char*)"_input", &Car::_input);
	register_method((char*)"_ready", &Car::_ready);
}

void Car::_init()
{

}

void Car::_process(float delta)
{

}

void Car::_input(InputEvent* e)
{

}

void Car::_ready()
{

}


Car::Car()
{
CO2_output = 0; // co2 output for the whole duration of simulation
maintenance = 0; // maintenance cost for the whole duration of simulation
fuel_input = 0; // fuel needed for the whole duration of simulation
// random device class instance, source of 'true' randomness for initializing random seed
std::random_device rd; 
// Mersenne twister PRNG, initialized with seed from previous random device instance
std::mt19937 gen(rd()); 
std::normal_distribution <double> d(36000, 18000);
cost = d(gen); // cost of 1 car in euros, randomised using gaussian
capacity = 5;  // capacity for cars if fixed
std::normal_distribution <double> d(0.5, 0.30);
occupancy_rate = d(gen); // average percentage occupancy of the car: number of people in the car / capacity
if (occupancy_rate>1) occupancy_rate=1;
std::normal_distribution <double> d(1, 0.5);
building_time = d(gen); // building time of 1 car, very fast
double alpha = (cost-36000)/36000;
if (alpha < 1) alpha =1;
satisfaction = 8.5 + alpha *1.5; //satisfaction is rather high, positive correlation with cost 
std::normal_distribution <double> d(50, 20);
km_perday = d(gen); // average km per day for this car
passengers = 0; //total number of passengers that used the car
}

void Car::simulate_step(double days)
{
fuel_input+=9.5/100*km_perday*days; //litres of fuel for car
CO2_output+=130/1000*km_perday*days; // co2 emissions per car
passengers+=capacity*occupancy_rate*days; //number of people that used the car in given period
double alpha = (cost-36000)/36000;
if (alpha < 1) alpha =1;
maintenance = (0.35+alpha*0.15)*km_perday*days; // maintenance cost depends on number of km
}

Car::~Car()
{

}

