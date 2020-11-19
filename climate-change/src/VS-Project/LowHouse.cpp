#include "LowHouse.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void LowHouse::_register_methods()
{
	register_method((char*)"_process", &LowHouse::_process);
	register_method((char*)"_input", &LowHouse::_input);
	register_method((char*)"_ready", &LowHouse::_ready);
}

void LowHouse::_init()
{

}

void LowHouse::_process(float delta)
{

}

void LowHouse::_input(InputEvent* e)
{

}

void LowHouse::_ready()
{

}


LowHouse::LowHouse()
{

}

LowHouse::~LowHouse()
{

}