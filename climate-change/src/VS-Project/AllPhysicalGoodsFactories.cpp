#include "AllPhysicalGoodsFactories.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void GoodsFactories::_register_methods()
{
	register_method((char*)"_process", &GoodsFactories::_process);
	register_method((char*)"_input", &GoodsFactories::_input);
	register_method((char*)"_ready", &GoodsFactories::_ready);
}

void GoodsFactories::_init()
{

}

void GoodsFactories::_process(float delta)
{

}

void GoodsFactories::_input(InputEvent* e)
{

}

void GoodsFactories::_ready()
{

}


GoodsFactories::GoodsFactories()
{

}

GoodsFactories::~GoodsFactories()
{

}