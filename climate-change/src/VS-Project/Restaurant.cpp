#include "Restaurant.h"
#include <Area.hpp>
#include <Camera.hpp>
#include <ClippedCamera.hpp>
#include <SceneTree.hpp>


using namespace godot;

// ((ClippedCamera*)get_node("/root")->get_child(0)->get_child(1)->get_child(0))->unproject_position(ObjPos)


Restaurant::Restaurant()
{
	PanelsOn = true;
	Clickable = false;
}

Restaurant::Restaurant(double income, double numberOfEmployees, double carbonEmission, double energyDemand, double energySupply, double healthcare, double needs)
{
	PanelsOn = true;
	Clickable = false;
	this->income = income;
	this->numberOfEmployees = numberOfEmployees;
	this->carbonEmission = carbonEmission;
	this->energyDemand = energyDemand;
	this->energySupply = energySupply;
	this->healthcare = healthcare;
	this->needs = needs;
}

void Restaurant::initialize() {
	this->income = 100;
	this->numberOfEmployees = 20;
	this->carbonEmission = 1000;
	this->energyDemand = 300;
	this->energySupply = 0;
	this->healthcare = 0;
	this->needs = 20;
}

double Restaurant::get_emissions(){
	double temp = this->carbonEmission;
	if (this->PanelsOn) {
		temp *= 0.5;
	}
	std::cout << "DEBUG: GET EMISSIONS CALLED" << std::endl;
	return temp;
}

Node* Restaurant::GetPanels()
{
	return get_node("Restau")->get_node("SolarPanels");
}

void Restaurant::_register_methods()
{
	register_method((char*)"_process", &Restaurant::_process);
	register_method((char*)"_input", &Restaurant::_input);
	register_method((char*)"_ready", &Restaurant::_ready);
	register_method((char*)"_on_Area_mouse_entered", &Restaurant::_on_Area_mouse_entered);
	register_method((char*)"_on_Area_mouse_exited", &Restaurant::_on_Area_mouse_exited);
}



void Restaurant::_init() {

}

void Restaurant::_process(float delta) {

}

void Restaurant::_input(InputEvent* e) {
	if (e->is_action_pressed("ui_select") && Clickable) {
		PanelsOn = (PanelsOn == false);
		this->GetPanels()->set("visible", PanelsOn);
	}
}


void Restaurant::_ready()
{
	this->GetPanels()->set("visible", PanelsOn);
}

void godot::Restaurant::_on_Area_mouse_entered()
{
	Clickable = true;											
	Input* i = Input::get_singleton();							// CHANGE MOUSE CURSOR
	i->set_default_cursor_shape(i->CURSOR_POINTING_HAND);
}



void godot::Restaurant::_on_Area_mouse_exited()
{
	Clickable = false;											
	Input* i = Input::get_singleton();							// CHANGE MOUSE CURSOR
	i->set_default_cursor_shape(i->CURSOR_ARROW);
}

