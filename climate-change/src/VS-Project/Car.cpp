#include "Car.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

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

}

Car::~Car()
{

}

