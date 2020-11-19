#include "HighBuilding.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void HighBuilding::_register_methods()
{
	register_method((char*)"_process", &HighBuilding::_process);
	register_method((char*)"_input", &HighBuilding::_input);
	register_method((char*)"_ready", &HighBuilding::_ready);
}

void HighBuilding::_init()
{

}

void HighBuilding::_process(float delta)
{

}

void HighBuilding::_input(InputEvent* e)
{

}

void HighBuilding::_ready()
{

}


HighBuilding::HighBuilding()
{

}

HighBuilding::~HighBuilding()
{

}