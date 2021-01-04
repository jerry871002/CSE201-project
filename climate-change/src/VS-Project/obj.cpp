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
#include <random>
#include <Math.hpp>




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

void Structure::set_satisfaction(double sat) {
    this->satisfaction = sat;
}

double Structure::get_co2emissions() {
    //std::cout << "DEBUG: STRUCTURE GET EMISSIONS" << std::endl;
    return this->CO2Emission;
}
void Structure::set_co2emissions(double emission) {
    this->CO2Emission = emission;
}

double Structure::get_energyuse() {
    return this->energyUse;
}
void Structure::set_energyuse(double energyUse) {
    this->energyUse = energyUse;
}

double Structure::get_environmental_cost() {
    return this->environmentalCost;
}
void Structure::set_environmental_cost(double environmentalCost) {
    this->environmentalCost = environmentalCost;
}

double Structure::get_cost() {
    return this->cost;
}
void Structure::set_cost(double cost) {
    this->cost = cost;
}

double Structure::get_employment() {
    return this->employment;
}
void Structure::set_employment(double employment) {
    this->employment = employment;
}

double Structure::get_building_time() {
    return this->buildingTime;
}
void Structure::set_building_time(double buildingTime) {
    this->buildingTime = buildingTime;
}

double Structure::get_maintenance() {
    return this->maintenance;
}
void Structure::set_maintenance(double maintenance) {
    this->maintenance = maintenance;
}

void Structure::_register_methods()
{
    register_method((char*)"_process", &Structure::_process);
    register_method((char*)"_input", &Structure::_input);
    register_method((char*)"_ready", &Structure::_ready);
    register_method((char*)"_on_Area_mouse_entered", &Structure::_on_Area_mouse_entered);
    register_method((char*)"_on_Area_mouse_exited", &Structure::_on_Area_mouse_exited);

    register_property<Structure, String>("object_type", &Structure::set_object_type, &Structure::get_type, "Structure");

    register_property<Structure, bool>("updatable", &Structure::updatable, false);

    register_property<Structure, double>("CO2Emission", &Structure::set_co2emissions, &Structure::get_co2emissions, 1);
    register_property<Structure, double>("satisfaction", &Structure::set_satisfaction, &Structure::get_satisfaction, 1);
    register_property<Structure, double>("energyUse", &Structure::set_energyuse, &Structure::get_energyuse, 1);
    register_property<Structure, double>("maintenance", &Structure::set_maintenance, &Structure::get_maintenance, 1);
    register_property<Structure, double>("employment", &Structure::set_employment, &Structure::get_employment, 1);
    register_property<Structure, double>("cost", &Structure::set_cost, &Structure::get_cost, 1);
    register_property<Structure, double>("environmentalCost", &Structure::set_environmental_cost, &Structure::get_environmental_cost, 1);
    register_property<Structure, double>("buildingTime", &Structure::set_building_time, &Structure::get_building_time, 1);

    //register_method((char*)"get_co2emissions", &Structure::get_co2emissions);

}

void Structure::_init()
{

}

Vector3 Structure::get_position() {
    return (Vector3)((Spatial*)this)->get_translation();
}

bool Structure::is_other_structure_within_distance(Vector3 other, double distance) {
    Vector3 pos = get_position();
    return  (sqrtf(other[0] * pos[0] + other[1] * pos[1] + other[2] * pos[2]) <= distance);
}

void Structure::_process(float delta)
{
    
    if (this->get("updatable")) {
        Godot::print("This is a: " + this->get_object_type());
        this->simulate_step((double)(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("time_speed"))); // will run the lowest level simulate step function
        this->set("updatable", false);
    }

    if (this->hover_animation_active) {
        ++(this->hover_animation_counter);
        this->set("scale", Vector3(object_scale.x, object_scale.y * float(1 + 0.25 * sin(M_PI *  hover_animation_counter / 5)), object_scale.z));

        if (this->hover_animation_counter == 5) {
            this->set("scale", object_scale);
            hover_animation_active = false;
            hover_animation_counter = 0;
        }
    }
    
}

void Structure::simulate_step(double days) {
    //std::cout << "DEBUG: STRUCTURE SIMULATION CALLED" << std::endl;
    //std::cout << "DEBUG: STRUCTURE SIMULATION DONE" << std::endl;
}

/*
void Structure::test_update() {
    Godot::print("The TEST UPDATE function is running");
    Godot::print("This is a structure but also a " + this->get_object_type());
}
*/

void Structure::_input(InputEvent* e)
{
    Input* i = Input::get_singleton();


    if (i->is_action_just_pressed("ui_select") && Clickable) {

        // RECORD MOUSE POSITION

        Vector2 mousePos = this->get_viewport()->get_mouse_position();

        // PAUSE GAME

        ((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->set("time_speed", 0);

        // PLAYER SHOULD NOT BE MOVABLE

        ((Player*)(this->get_tree()->get_root()->get_node("Main/3Dworld/Player")))->set("movable", false);

        // SET INFO BOX SIZE AND POSITION

        ((Label*)(this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")))->set("rect_size", Vector2(InfoBoxWidth, (get_viewport()->get_size().y) - 260));

        if (mousePos.x > (get_viewport()->get_size().x) / 2) 
        {
            (this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox"))->set("rect_position", Vector2(60, 200));
        }
        else {
            real_t AdaptedWidth = ((Vector2)(((Label*)(this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")))->get("rect_size"))).x;
            (this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox"))->set("rect_position", Vector2(get_viewport()->get_size().x - AdaptedWidth - 60, 200));
        }

        // AJUST POSITION OF MENU TO ENSURE IT IS VISIBLE

        if (get_viewport()->get_size().x - mousePos.x <= MenuSize)
        {
            if (mousePos.y > (get_viewport()->get_size().y / 2)) { mousePos.y -= MenuSize - (get_viewport()->get_size().x - mousePos.x); }
            else { mousePos.y += MenuSize - (get_viewport()->get_size().x - mousePos.x); }
            mousePos.x = get_viewport()->get_size().x - MenuSize;
        }
        else if (mousePos.x <= MenuSize)
        {
            if (mousePos.y > (get_viewport()->get_size().y / 2)) { mousePos.y -= MenuSize - mousePos.x; }
            else { mousePos.y += MenuSize - mousePos.x; }
            mousePos.x = MenuSize;
        }

        if (get_viewport()->get_size().y - mousePos.y <= MenuSize) 
        {
            if (mousePos.x > (get_viewport()->get_size().x / 2)) { mousePos.x -= MenuSize - (get_viewport()->get_size().y - mousePos.y); }
            else { mousePos.x += MenuSize - (get_viewport()->get_size().y - mousePos.y); }
            mousePos.y = get_viewport()->get_size().y - MenuSize;
        }
        else if (mousePos.y <= MenuSize)
        {
            if (mousePos.x > (get_viewport()->get_size().x / 2)) { mousePos.x -= MenuSize - mousePos.y; }
            else { mousePos.x += MenuSize - mousePos.y; }
            mousePos.y = MenuSize;
        }

        (this->get_tree()->get_root()->get_node("Main/2Dworld/Menus"))->set("position", mousePos);


        show_menu();
    }
}

void Structure::show_menu()
{
    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("text", get_object_info());
    this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", true);

    godot::String MenuPathString = "Main/2Dworld/Menus/Menu" + this->get_main_type();
    NodePath MenuPath = (NodePath)MenuPathString;

    if (this->get_tree()->get_root()->has_node(MenuPath)) {
        this->get_tree()->get_root()->get_node(MenuPath)->set("visible", true);
    }
    else {
        Godot::print("A node was not found at the following path: ");
        Godot::print(MenuPathString);
    }

    
}

template<typename T> String to_godot_string(T s)
{
    std::string standardString = std::to_string(s);
    godot::String godotString = godot::String(standardString.c_str());
    return godotString;
}

String Structure::get_object_info()
{
    String info = String("INFORMATION") + String("\n");
    info += this->get_main_type() + String(" >> ") + this->get_object_type() + String("\n");
    return  info;
}

void Structure::_ready()
{
    object_scale = this->get("scale");
}

void godot::Structure::_on_Area_mouse_entered()
{

    Clickable = true;
    Input* i = Input::get_singleton();
    i->set_default_cursor_shape(i->CURSOR_POINTING_HAND);
    this->hover_animation_active = true;
    //this->set("scale", Vector3(Vector3(this->get("scale")).x, 1.25 * Vector3(this->get("scale")).y, Vector3(this->get("scale")).z));

}

void godot::Structure::_on_Area_mouse_exited()
{
    Clickable = false;
    Input* i = Input::get_singleton();
    i->set_default_cursor_shape(i->CURSOR_ARROW);
    //this->set("scale", Vector3(Vector3(this->get("scale")).x, (1/1.25) * Vector3(this->get("scale")).y, Vector3(this->get("scale")).z));
    this->set("scale", object_scale);
    hover_animation_active = false;
    hover_animation_counter = 0;
}

//POLICIES (interface team needs to make them usable from screen)

bool Structure::efficiency_cogeneration() {
    return false; 
    /* need to add possibility of turning true (then definitive) if clicked
    if () {
    return true;
    }*/
}

bool Structure::efficiency_supercritical() {
    return false;
    /* need to add possibility of turning true (then definitive) if clicked
    if () {
    return true;
    }*/
}

bool Structure::coal_prohibited() {
    return false;
    /* need to add possibility of turning true (not definitive) if clicked
    if () {
    return true;
    }*/
}

bool Structure::nuclear_prohibited() {
    return false;
    /* need to add possibility of turning true (not definitive) if clicked
    if () {
    return true;
    }*/
}

void Structure::maximum_CO2() {
    maxi = 10E6; //basic value to compare with when the policy is not implemented
    /* need to add possibility of turning true (not definitive) if clicked with given input which is the "maxi" possible value emitted the function returns
    if () {
    maxi = ...; //maxi is an input between 0 and 4250 
    policy_change = true; //very important to have the change in the policy taken into account !
    }*/
}

double Structure::subsidy_green() {
    return 0; //basic return value to compare with when the policy is not implemented
    /* need to add possibility of turning true (not definitive) if clicked with given input which is the "euros" given to the factories which
    this function returns as a double between 1000 and 1 million 
    if () {
    return euros; //euros is between 1000 and 1E6
    }*/
}

bool Structure::solar_panel_subsidies() {
    return false;
    //possibility to change this and have: if true set solarPanelSubsidy to a number
    /* need to add possibility of turning true (not definitive) if clicked
    if () {
    return true;
    }*/
}

bool Structure::wind_turbine_subsidies() {
    return false;
    /* need to add possibility of turning true (not definitive) if clicked
    if () {
    return true;
    }*/
}


bool Structure::double_glazing_subsidies(){
    return false;
    /* need to add possibility of turning true (not definitive) if clicked
    if () {
    return true;
    }*/
}