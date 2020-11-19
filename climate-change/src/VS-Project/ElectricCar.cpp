#include "ElectricCar.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void ElectricCar::_register_methods()
{
	register_method((char*)"_process", &ElectricCar::_process);
	register_method((char*)"_input", &ElectricCar::_input);
	register_method((char*)"_ready", &ElectricCar::_ready);
}

void ElectricCar::_init()
{

}

void ElectricCar::_process(float delta)
{

}

void ElectricCar::_input(InputEvent* e)
{

}

void ElectricCar::_ready()
{

}


ElectricCar::ElectricCar()
{

}

ElectricCar::~ElectricCar()
{

}