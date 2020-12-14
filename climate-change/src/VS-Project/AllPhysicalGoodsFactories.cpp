#include "AllPhysicalGoodsFactories.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

String godot::AllPhysicalGoodsFactories::class_name()
{
	return "AllPhysicalGoodsFactories";
}

void AllPhysicalGoodsFactories::_register_methods()
{
	register_method((char*)"_init", &AllPhysicalGoodsFactories::_init);
	register_method((char*)"_process", &AllPhysicalGoodsFactories::_process);
	register_method((char*)"_input", &AllPhysicalGoodsFactories::_input);
	register_method((char*)"_ready", &AllPhysicalGoodsFactories::_ready);
}

void AllPhysicalGoodsFactories::_init()
{

}

void AllPhysicalGoodsFactories::_process(float delta)
{

}

void AllPhysicalGoodsFactories::_input(InputEvent* e)
{

}

void AllPhysicalGoodsFactories::_ready()
{

}


AllPhysicalGoodsFactories::AllPhysicalGoodsFactories()
{

}

AllPhysicalGoodsFactories::~AllPhysicalGoodsFactories()
{

}