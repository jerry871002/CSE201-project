#include <iostream>
#include "obj.h"
#include <GodotGlobal.hpp>
#include <Viewport.hpp>
#include <StaticBody2D.hpp>
#include <SceneTree.hpp>
#include <Rect2.hpp>
#include <Label.hpp>
#include <String.hpp>
#include <wchar.h>
#include <string>
#include <stdlib.h>
#include "City.h"
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
    register_method((char*)"simulate_step", &Structure::simulate_step);
}

void Structure::_init()
{
    std::cout << "DEBUG: STRUCTURE INIT CALLED" << std::endl;

}

void Structure::simulate_step(double days) 
{
    std::cout << "HELLO THERE STRUCTURE SIMULATION" << std::endl;
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

        std::cout << "DEBUG: STRUCTURE OBJECT CLICKED" << std::endl;

        ((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->set("time_speed", 0);

        Vector2 mousePos = this->get_viewport()->get_mouse_position();

        ((Player*)(this->get_tree()->get_root()->get_node("Main/3Dworld/Player")))->set("movable", false);
        std::cout << "PLAYER SHOULD NOT BE MOVABLE" << std::endl;

        ((Label*)(this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")))->set("rect_size", Vector2(InfoBoxWidth, (get_viewport()->get_size().y) - 260));

        if (mousePos.x > (get_viewport()->get_size().x) / 2) {
            (this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox"))->set("rect_position", Vector2(60, 200));
        }
        else {
            real_t AdaptedWidth = ((Vector2)(((Label*)(this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")))->get("rect_size"))).x;
            (this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox"))->set("rect_position", Vector2(get_viewport()->get_size().x - AdaptedWidth - 60, 200));
        }

        if (get_viewport()->get_size().x - mousePos.x <= MenuSize) 
        {
            if (mousePos.y > (get_viewport()->get_size().y / 2)) {mousePos.y -= MenuSize - (get_viewport()->get_size().x - mousePos.x);}
            else {mousePos.y += MenuSize - (get_viewport()->get_size().x - mousePos.x);}
            mousePos.x = get_viewport()->get_size().x - MenuSize;
        }
        else if (mousePos.x <= MenuSize)
        {
            if (mousePos.y > (get_viewport()->get_size().y / 2)) {mousePos.y -= MenuSize  - mousePos.x;}
            else {mousePos.y += MenuSize - mousePos.x;}
            mousePos.x = MenuSize;
        }

        if (get_viewport()->get_size().y - mousePos.y <= MenuSize) {
            
            if (mousePos.x > (get_viewport()->get_size().x / 2)) {mousePos.x -= MenuSize - (get_viewport()->get_size().y - mousePos.y);}
            else {mousePos.x += MenuSize - (get_viewport()->get_size().y - mousePos.y);}
            mousePos.y = get_viewport()->get_size().y - MenuSize;
        }
        else if (mousePos.y <= MenuSize)
        {
            if (mousePos.x > (get_viewport()->get_size().x / 2)) {mousePos.x -= MenuSize - mousePos.y;}
            else {mousePos.x += MenuSize - mousePos.y;}
            mousePos.y = MenuSize;
        }

        (this->get_tree()->get_root()->get_node("Main/2Dworld/Menus"))->set("position", mousePos);
        
		show_menu();
	}
}

void Structure::show_menu() 
{ 

    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("text", output_information());

    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", true); 
}

template<typename T> String to_godot_string(T s)
{
    std::string standardString = std::to_string(s);
    godot::String godotString = godot::String(standardString.c_str());
    return godotString;
}

//template String to_godot_string<double>(double);
//template String to_godot_string<int>(int);
//template String to_godot_string<float>(float);
//template String to_godot_string<real_t>(real_t);
//template String to_godot_string<bool>(bool);

String Structure::output_information() 
{
    int h = 1;
    std::string y = "HELLO";
    String s = String("\n Employment: ");
    std::string standardString = std::to_string(h);
    godot::String godotString = godot::String(standardString.c_str());
    std::string s2 = "hello";
    
    return String("EMPLOYMENT: ") + to_godot_string(employment);
}

void Structure::_ready()
{

    std::cout << "DEBUG: STRUCTURE READY CALLED" << std::endl;

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
