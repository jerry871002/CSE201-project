#include <iostream>
#include "obj.h"
#include <GodotGlobal.hpp>


using namespace godot;

Structure::Structure() {

    MenuVisible = false;
    Clickable = false;
    /*
    cost = 0;
    energyUse = 0;
    maintenance = 0;
    satisfaction = 0;
    income = 0;
    population = 0;
    numberOfEmployees = 0;
    carbonEmission = 0;
    energyDemand = 0;
    energySupply = 0;
     */
    
 }
/*
Structure::Structure(double cost, double energyUse, double maintenance, double satisfaction, double income, double population, double numberOfEmployees, double carbonEmission, double energyDemand, double energySupply) :
    cost{ cost }, energyUse{ energyUse }, maintenance{ maintenance }, satisfaction{ satisfaction }, income{ income }, population{ population }, numberOfEmployees{ numberOfEmployees }, carbonEmission{ carbonEmission }, energyDemand{ energyDemand }, energySupply{ energySupply }{}
*/

Structure::~Structure() {}

void Structure::_register_methods()
{
    register_method((char*)"_process", &Structure::_process);
    register_method((char*)"_input", &Structure::_input);
    register_method((char*)"_ready", &Structure::_ready);
    register_method((char*)"_on_Area_mouse_entered", &Structure::_on_Area_mouse_entered);
    register_method((char*)"_on_Area_mouse_exited", &Structure::_on_Area_mouse_exited);
    // register_method((char*)"_on_CheckBox_pressed", &Structure::_on_CheckBox_pressed);
    // register_method((char*)"_on_CheckBox_button_up", &Structure::_on_CheckBox_button_up);
    // register_method((char*)"_on_CheckBox_button_down", &Structure::_on_CheckBox_button_down);
    // register_method((char*)"_on_CheckBox_toggled", &Structure::_on_CheckBox_toggled);
}

void Structure::_init()
{

}

Vector3 Structure::get_position() {
    return (Vector3)((Spatial*)this)->get_translation();
}

bool Structure::is_other_structure_within_distance(Vector3 other, double distance) {
    Vector3 pos =  get_position();
    return  (sqrtf(other[0] * pos[0] +other[1]*pos[1] + other[2] * pos[2]) <= distance);
}

void Structure::_process(float delta)
{

}

void Structure::_input(InputEvent* e)
{
	Input* i = Input::get_singleton();

	if (i->is_action_pressed("ui_select") && Clickable) {
		//PanelsOn = (PanelsOn == false);
		//this->GetPanels()->set("visible", PanelsOn);
		//this->get_child(0)->set("pressed", PanelsOn);
		//MenuVisible = (MenuVisible == false);
		//Bthis->get_child(0)->set("rect_position", Vector2(this->get_viewport()->get_mouse_position().x, this->get_viewport()->get_mouse_position().x));
		//this->get_child(0)->set("visible", MenuVisible);
	}
}


void Structure::_ready()
{
	//this->GetPanels()->set("visible", PanelsOn);
	//this->get_child(0)->set("pressed", PanelsOn);
	//this->get_child(0)->set("text", this->class_name() + ": Menu");
	//this->get_child(0)->set("visible", MenuVisible);
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

/*


void godot::Structure::_on_CheckBox_pressed()
{

}

void godot::Structure::_on_CheckBox_toggled()
{
	PanelsOn = (PanelsOn == false);
	this->GetPanels()->set("visible", PanelsOn);
}


void godot::Structure::_on_CheckBox_button_up()
{

}

void godot::Structure::_on_CheckBox_button_down()
{

}

*/

String godot::Structure::class_name()
{
	return "Structure";
}

bool Structure::efficiency_cogeneration() {
    return false; //need to add possibility of turning true (then definitive) if clicked, adding the cost in maintenace needed for this upgrade
}

bool Structure::efficiency_supercritical() {
    return false; //need to add possibility of turning true (then definitive) if clicked, adding the cost in maintenace needed for this upgrade
}
