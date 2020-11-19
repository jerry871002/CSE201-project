#include "CoalPowerPlant.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Coal::_register_methods()
{
	register_method((char*)"_process", &Coal::_process);
	register_method((char*)"_input", &Coal::_input);
	register_method((char*)"_ready", &Coal::_ready);
}

void Coal::_init()
{

}

void Coal::_process(float delta)
{

}

void Coal::_input(InputEvent* e)
{

}

void Coal::_ready()
{

}


Coal::Coal()
{

}

Coal::~Coal()
{

}