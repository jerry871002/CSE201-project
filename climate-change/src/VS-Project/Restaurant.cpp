#include "Restaurant.h"
#include "random"
#include <Area.hpp>


using namespace godot;


Restaurant::Restaurant() // : Structure()
{
	income = 100;
}

Restaurant::~Restaurant()
{

}

void Restaurant::_register_methods()
{
	register_method((char*)"_process", &Restaurant::_process);
	register_method((char*)"_input", &Restaurant::_input);
	register_method((char*)"_ready", &Restaurant::_ready);
}

void Restaurant::change_income_by_percent(float factor ) {
	income *= factor;
}

void Restaurant::_init() 
{
}

void Restaurant::_process(float delta)
{
}

void Restaurant::_input(Input* e)
{
	
}

void Restaurant::_ready()
{
	
}



