#include "Structure.h"
#include "random"
#include <Area.hpp>


using namespace godot;


Structure::Structure()
{
	PanelsOn = true;
	Clickable = false;
}

Structure::~Structure()
{

}

void Structure::_register_methods()
{
	register_method((char*)"_process", &Structure::_process);
	register_method((char*)"_input", &Structure::_input);
	register_method((char*)"_ready", &Structure::_ready);
	register_method((char*)"_on_Area_mouse_entered", &Structure::_on_Area_mouse_entered);
	register_method((char*)"_on_Area_mouse_exited", &Structure::_on_Area_mouse_exited);
}

void Structure::_init()
{

}

void Structure::_process(float delta)
{

}

void Structure::_input(Input* e)
{
	if (e->get_class() == "InputEventMouseButton" && Clickable) 
	{
		if (((InputEventMouseButton*)e)->is_pressed()) 
		{ 
			PanelsOn = (PanelsOn == false);
			this->GetPanels()->set("visible", PanelsOn);
		}
	}
}

void Structure::_ready()
{
	this->GetPanels()->set("visible", PanelsOn);
}

Node* Structure::GetPanels()
{
	return this->get_node("MeshComponents")->get_node("SolarPanels");
}

void godot::Structure::_on_Area_mouse_entered()
{

	Clickable = true;
	//this->GetPanels()->set("visible", true);

	Input* i = Input::get_singleton();
	// CHANGE MOUSE CURSOR
	i->set_default_cursor_shape(i->CURSOR_POINTING_HAND);

}

void godot::Structure::_on_Area_mouse_exited()
{
	Clickable = false;
	//this->GetPanels()->set("visible", false);
	Input* i = Input::get_singleton();
	// CHANGE MOUSE CURSOR
	i->set_default_cursor_shape(i->CURSOR_ARROW);

}


