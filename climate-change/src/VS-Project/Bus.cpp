#include "Bus.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

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

}

Bus::~Bus()
{

}