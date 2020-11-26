#include "Restaurant.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;


Restaurant::Restaurant()
{

}

Restaurant::~Restaurant(){}

void Restaurant::_register_methods()
{
	register_method((char*)"_process", &Restaurant::_process);
	register_method((char*)"_input", &Restaurant::_input);
	register_method((char*)"_ready", &Restaurant::_ready);
}

void Restaurant::_init()
{

}

void Restaurant::_process(float delta)
{

}

void Restaurant::_input(InputEvent* e)
{

}

void Restaurant::_ready()
{

}



double Restaurant::get_shock(double upper_bound){
	return 0;
}  

double Restaurant::get_energyuse(){
	return 0;
}

double Restaurant::get_popularity(){
	return 0;
}

double Restaurant::get_world_counter_impact(){
	return 0;
}


void Restaurant::calculate_demand(){
	double res;
	res = ((*this).owner_skills*10 + (*this).popularity*30 + (*this).demand*40 + (*this).get_world_counter_impact()*20 ) / 100;
	double upper_bound = 100-res;
	res = res + (*this).get_shock(upper_bound);
	(*this).demand = res;
}


void Restaurant::calculate_costs(){
	double res;

}
