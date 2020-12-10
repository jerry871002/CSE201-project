#include "SmallShop.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void SmallShop::_register_methods()
{
	register_method((char*)"_init", &SmallShop::_init);
	register_method((char*)"_process", &SmallShop::_process);
	register_method((char*)"_input", &SmallShop::_input);
	register_method((char*)"_ready", &SmallShop::_ready);
}

void SmallShop::_init()
{

}

void SmallShop::_process(float delta)
{

}

void SmallShop::_input(InputEvent* e)
{

}

void SmallShop::_ready()
{

}


SmallShop::SmallShop()
{

}

SmallShop::~SmallShop()
{

}