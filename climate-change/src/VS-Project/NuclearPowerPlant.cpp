#include "NuclearPowerPlant.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Nuclear::_register_methods()
{
	register_method((char*)"_process", &Nuclear::_process);
	register_method((char*)"_input", &Nuclear::_input);
	register_method((char*)"_ready", &Nuclear::_ready);
}

void Nuclear::_init()
{

}

void Nuclear::_process(float delta)
{

}

void Nuclear::_input(InputEvent* e)
{

}

void Nuclear::_ready()
{

}


Nuclear::Nuclear()
{

}

Nuclear::~Nuclear()
{

}