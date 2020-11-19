#include "AgriculturalProduction.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void AgriculturalProduction::_register_methods()
{
	register_method((char*)"_process", &AgriculturalProduction::_process);
	register_method((char*)"_input", &AgriculturalProduction::_input);
	register_method((char*)"_ready", &AgriculturalProduction::_ready);
}

void AgriculturalProduction::_init()
{

}

void AgriculturalProduction::_process(float delta)
{

}

void AgriculturalProduction::_input(InputEvent* e)
{

}

void AgriculturalProduction::_ready()
{

}


AgriculturalProduction::AgriculturalProduction()
{

}

AgriculturalProduction::~AgriculturalProduction()
{

}
