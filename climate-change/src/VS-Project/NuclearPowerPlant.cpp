#include "NuclearPowerPlant.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void NuclearPowerPlant::_register_methods()
{
	register_method((char*)"_process", &NuclearPowerPlant::_process);
	register_method((char*)"_input", &NuclearPowerPlant::_input);
	register_method((char*)"_ready", &NuclearPowerPlant::_ready);
}

void NuclearPowerPlant::_init()
{

}

void NuclearPowerPlant::_process(float delta)
{

}

void NuclearPowerPlant::_input(InputEvent* e)
{

}

void NuclearPowerPlant::_ready()
{

}


NuclearPowerPlant::NuclearPowerPlant()
{

}

NuclearPowerPlant::~NuclearPowerPlant()
{

}