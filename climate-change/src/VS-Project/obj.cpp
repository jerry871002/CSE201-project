#include <iostream>
#include "obj.h"
#include <GodotGlobal.hpp>
#include <Viewport.hpp>
#include <StaticBody2D.hpp>
#include <SceneTree.hpp>
#include "Player.h"


using namespace godot;

Structure::Structure() {

    MenuVisible = false;
    Clickable = false;
    totalDays = 0;
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

double Structure::get_satisfaction() {
	return this->satisfaction;
}

double Structure::get_co2emissions() {
	return this->CO2Emission;
}

double Structure::get_energyuse() {
	return this->energyUse;
}

double Structure::get_environmentalcost() {
	return this->environmentalCost;
}

double Structure::get_cost() {
	return this->cost;
}

double Structure::get_employment() {
	return this->employment;
}

double Structure::get_building_time() {
	return this->buildingTime;
}

double Structure::get_maintenance() {
	return this->maintenance;
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
        Vector2 mousePos = this->get_viewport()->get_mouse_position();
        std::cout << mousePos.x;
        (this->get_tree()->get_root()->get_node("Main/2Dworld/Menus"))->set("position", mousePos);
		show_menu();
	}
}

void Structure::show_menu() {}


void Structure::_ready()
{

}

void godot::Structure::_on_Area_mouse_entered()
{

	Clickable = true;
	Input* i = Input::get_singleton();
	i->set_default_cursor_shape(i->CURSOR_POINTING_HAND);


}

void godot::Structure::_on_Area_mouse_exited()
{
	Clickable = false;
	Input* i = Input::get_singleton();
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
