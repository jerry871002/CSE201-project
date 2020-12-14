#include "RecyclingFactory.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;
String godot::RecyclingFactory::class_name()
{
	return "RecyclingFactory";
}

void RecyclingFactory::_register_methods()
{
	register_method((char*)"_init", &RecyclingFactory::_init);
	register_method((char*)"_process", &RecyclingFactory::_process);
	register_method((char*)"_input", &RecyclingFactory::_input);
	register_method((char*)"_ready", &RecyclingFactory::_ready);
}

void RecyclingFactory::_init()
{

}

void RecyclingFactory::_process(float delta)
{

}

void RecyclingFactory::_input(InputEvent* e)
{

}

void RecyclingFactory::_ready()
{

}


RecyclingFactory::RecyclingFactory()
{

}

RecyclingFactory::~RecyclingFactory()
{

}