#include "Tram.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Tram::_register_methods()
{
	register_method((char*)"_process", &Tram::_process);
	register_method((char*)"_input", &Tram::_input);
	register_method((char*)"_ready", &Tram::_ready);
}

void Tram::_init()
{

}

void Tram::_process(float delta)
{

}

void Tram::_input(InputEvent* e)
{

}

void Tram::_ready()
{

}


Tram::Tram()
{

}

Tram::~Tram()
{

}