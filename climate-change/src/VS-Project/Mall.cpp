#include "Mall.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Mall::_register_methods()
{
	register_method((char*)"_init", &Mall::_init);
	register_method((char*)"_process", &Mall::_process);
	register_method((char*)"_input", &Mall::_input);
	register_method((char*)"_ready", &Mall::_ready);
}

void Mall::_init()
{

}

void Mall::_process(float delta)
{

}

void Mall::_input(InputEvent* e)
{

}

void Mall::_ready()
{

}


Mall::Mall()
{

}

Mall::~Mall()
{

}