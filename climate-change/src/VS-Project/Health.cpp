#include "Health.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

String godot::Health::class_name()
{
	return "Health";
}
void Health::_register_methods()
{
	register_method((char*)"_init", &Health::_init);
	register_method((char*)"_process", &Health::_process);
	register_method((char*)"_input", &Health::_input);
	register_method((char*)"_ready", &Health::_ready);
}

void Health::_init()
{

}

void Health::_process(float delta)
{

}

void Health::_input(InputEvent* e)
{

}

void Health::_ready()
{

}


Health::Health()
{

}

Health::~Health()
{

}