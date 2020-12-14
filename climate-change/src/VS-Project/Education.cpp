#include "Education.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;
String godot::Education::class_name()
{
	return "Education";
}

void Education::_register_methods()
{
	register_method((char*)"_init", &Education::_init);
	register_method((char*)"_process", &Education::_process);
	register_method((char*)"_input", &Education::_input);
	register_method((char*)"_ready", &Education::_ready);
}

void Education::_init()
{

}

void Education::_process(float delta)
{

}

void Education::_input(InputEvent* e)
{

}

void Education::_ready()
{

}


Education::Education()
{

}

Education::~Education()
{

}