#include "HighHouse.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void HighHouse::_register_methods()
{
	register_method((char*)"_process", &HighHouse::_process);
	register_method((char*)"_input", &HighHouse::_input);
	register_method((char*)"_ready", &HighHouse::_ready);
}

void HighHouse::_init()
{

}

void HighHouse::_process(float delta)
{

}

void HighHouse::_input(InputEvent* e)
{

}

void HighHouse::_ready()
{

}


HighHouse::HighHouse()
{

}

HighHouse::~HighHouse()
{

}