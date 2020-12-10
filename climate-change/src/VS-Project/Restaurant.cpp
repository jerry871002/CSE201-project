#include "Restaurant.h"
#include <Area.hpp>
#include <Camera.hpp>
#include <ClippedCamera.hpp>
#include <SceneTree.hpp>


using namespace godot;

// ((ClippedCamera*)get_node("/root")->get_child(0)->get_child(1)->get_child(0))->unproject_position(ObjPos)


Restaurant::Restaurant()
{
	PanelsOn = false;
	Clickable = false;
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
	std::cout << "DEBUG: GET_EMISSIONS_CALLED" << std::endl;
	double temp = this->carbonEmission;
	std::cout << temp << std::endl;
	if (this->ArePanelsDisplayed()) {
		temp = 0.5*temp;
	}
	std::cout << "DEBUG: GET_EMISSIONS_DONE" << std::endl;
	std::cout << temp << std::endl;
	std::cout << this->ArePanelsDisplayed() << std::endl;
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
		this->PanelsOn = !(this->PanelsOn);
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

bool godot::Restaurant::ArePanelsDisplayed() 
{
	return this->GetPanels()->get("visible");
}