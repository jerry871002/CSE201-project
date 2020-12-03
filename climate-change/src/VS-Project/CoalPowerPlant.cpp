#include "CoalPowerPlant.h"
// #include <Math.hpp> do npt have that file on my directory 
#include <GodotGlobal.hpp>

using namespace godot;

void CoalPowerPlant::_register_methods()
{
	register_method((char*)"_process", &CoalPowerPlant::_process);
	register_method((char*)"_input", &CoalPowerPlant::_input);
	register_method((char*)"_ready", &CoalPowerPlant::_ready);
}

void CoalPowerPlant::_init()
{

}

void CoalPowerPlant::_process(float delta)
{

}

void CoalPowerPlant::_input(InputEvent* e)
{

}

void CoalPowerPlant::_ready()
{

}


CoalPowerPlant::CoalPowerPlant()
{

}

CoalPowerPlant::~CoalPowerPlant()
{

}