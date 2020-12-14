#include "Services.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;
String godot::Services::class_name()
{
	return "Services";
}

void Services::_register_methods()
{
	register_method((char*)"_init", &Services::_init);
	register_method((char*)"_process", &Services::_process);
	register_method((char*)"_input", &Services::_input);
	register_method((char*)"_ready", &Services::_ready);
}

void Services::_init()
{

}

void Services::_process(float delta)
{

}

void Services::_input(InputEvent* e)
{

}

void Services::_ready()
{

}


Services::Services()
{

}

Services::~Services()
{

}