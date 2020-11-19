#include "Windmill.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Windmill::_register_methods()
{
	register_method((char*)"_process", &Windmill::_process);
	register_method((char*)"_input", &Windmill::_input);
	register_method((char*)"_ready", &Windmill::_ready);
}

void Windmill::_init()
{

}

void Windmill::_process(float delta)
{

}

void Windmill::_input(InputEvent* e)
{

}

void Windmill::_ready()
{

}


Windmill::Windmill()
{

}

Windmill::~Windmill()
{

}