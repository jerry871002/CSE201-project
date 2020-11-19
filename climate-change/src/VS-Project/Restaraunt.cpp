#include "Restaraunt.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Restaraunt::_register_methods()
{
	register_method((char*)"_process", &Restaraunt::_process);
	register_method((char*)"_input", &Restaraunt::_input);
	register_method((char*)"_ready", &Restaraunt::_ready);
}

void Restaraunt::_init()
{

}

void Restaraunt::_process(float delta)
{

}

void Restaraunt::_input(InputEvent* e)
{

}

void Restaraunt::_ready()
{

}


Restaraunt::Restaraunt()
{

}

Restaraunt::~Restaraunt()
{

}