#include "Windmill.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Wind::_register_methods()
{
	register_method((char*)"_process", &Wind::_process);
	register_method((char*)"_input", &Wind::_input);
	register_method((char*)"_ready", &Wind::_ready);
}

void Wind::_init()
{

}

void Wind::_process(float delta)
{

}

void Wind::_input(InputEvent* e)
{

}

void Wind::_ready()
{

}


Wind::Wind()
{

}

Wind::~Wind()
{

}