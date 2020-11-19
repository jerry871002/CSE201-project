#include "RecyclingFactory.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Recycling::_register_methods()
{
	register_method((char*)"_process", &Recycling::_process);
	register_method((char*)"_input", &Recycling::_input);
	register_method((char*)"_ready", &Recycling::_ready);
}

void Recycling::_init()
{

}

void Recycling::_process(float delta)
{

}

void Recycling::_input(InputEvent* e)
{

}

void Recycling::_ready()
{

}


Recycling::Recycling()
{

}

Recycling::~Recycling()
{

}