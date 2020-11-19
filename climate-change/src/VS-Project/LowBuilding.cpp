#include "LowBuilding.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void LowBuilding::_register_methods()
{
	register_method((char*)"_process", &LowBuilding::_process);
	register_method((char*)"_input", &LowBuilding::_input);
	register_method((char*)"_ready", &LowBuilding::_ready);
}

void LowBuilding::_init()
{

}

void LowBuilding::_process(float delta)
{

}

void LowBuilding::_input(InputEvent* e)
{

}

void LowBuilding::_ready()
{

}


LowBuilding::LowBuilding()
{

}

LowBuilding::~LowBuilding()
{

}