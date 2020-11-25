#include "Bus.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <random>
#include <iostream>

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

age = 0;
std::normal_distribution <int> d(3125000, 62500);
cost = d(gen);
double alpha = (cost-250000)/250000;
if (alpha<0) alpha=0;
capacity = 30+ alpha*30;
capacity  = round(capacity);
std::normal_distribution <double> d(0.70, 0.30);
occupancy_rate = d(gen);
if (occupancy_rate>1) occupancy_rate=1;
std::normal_distribution <int> d(12, 2);
life_expectancy = d(gen);
std::normal_distribution <int> d(225, 75);
km_perday = d(gen);
CO2_output = km_perday * capacity * occupancy_rate * 70 /1000; // in  kilogramms per day!
fuel_perday = 26/100*km_perday; //litres per day
std::normal_distribution <int> d(3, 1);
employment = d(gen);
if (employment <1) employment =1;
maintenance = (0.8+alpha*0.2)*km_perday; // maintenance cost per day



}

Bus::~Bus()
{

}