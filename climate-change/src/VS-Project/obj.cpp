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

# define M_PI 3.14159265358979323846  /* pi */

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
    if ((this->get_main_type() == "Housing" || (this->get_main_type() == "Shop") && this->get_object_type() == "Mall" ) && (this->get_node("MeshComponents/Trees")->get("visible"))) {
        return ((this->satisfaction) + 4);
    }
    return this->satisfaction;
}

void Structure::set_satisfaction(double sat) {
    this->satisfaction = sat;
}


// these are only for houses
int Structure::get_inhabitants() {
    return this->numberOfInhabitants;
}

void Structure::set_inhabitants(int value) {
    this->numberOfInhabitants = value;
}

double Structure::get_age() {
    return this->age;
}

void Structure::set_age(double age) {
    this->age = age;
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

double Structure::get_energyperDay() {
    return this->energyperDay;
}
void Structure::set_energyperDay(double energyperDay) {
    this->energyperDay = energyperDay;
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

double Structure::get_averageWage() {
    return this->averageWage;
}
void Structure::set_averageWage(double averageWage) {
    this->averageWage = averageWage;
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
    register_property<Structure, double>("averageWage", &Structure::set_averageWage, &Structure::get_averageWage, 1);
    register_property<Structure, double>("maintenance", &Structure::set_maintenance, &Structure::get_maintenance, 1);
    register_property<Structure, double>("employment", &Structure::set_employment, &Structure::get_employment, 1);
    register_property<Structure, double>("cost", &Structure::set_cost, &Structure::get_cost, 1);
    register_property<Structure, double>("environmentalCost", &Structure::set_environmental_cost, &Structure::get_environmental_cost, 1);
    register_property<Structure, double>("buildingTime", &Structure::set_building_time, &Structure::get_building_time, 1);
    register_property<Structure, double>("age", &Structure::set_age, &Structure::get_age, 1);
    register_property<Structure, int>("numberOfInhabitants", &Structure::set_inhabitants, &Structure::get_inhabitants, 0);

    //register_method((char*)"get_co2emissions", &Structure::get_co2emissions);

    register_property<Structure, double>("solar_panel_subsidies", &Structure::solar_panel_subsidies, 0);
    register_property<Structure, double>("efficiency_supercritical", &Structure::efficiency_supercritical, 0);
    register_property<Structure, double>("efficiency_cogeneration", &Structure::efficiency_cogeneration, 0);
    register_property<Structure, double>("nuclear_prohibited", &Structure::nuclear_prohibited, 0);
    register_property<Structure, double>("coal_prohibited", &Structure::coal_prohibited, 0);
    register_property<Structure, double>("maximum_CO2", &Structure::maximum_CO2, 0);
    register_property<Structure, double>("subsidy_green", &Structure::subsidy_green, 0);
    register_property<Structure, double>("pesticideProhibited", &Structure::pesticideProhibited, 0);
    register_property<Structure, double>("GMOProhibited", &Structure::GMOProhibited, 0);
    register_property<Structure, double>("fertilizerProhibited", &Structure::fertilizerProhibited, 0);
    register_property<Structure, double>("solar_panel_subsidies_housing", &Structure::solar_panel_subsidies_housing, 0);
    register_property<Structure, double>("wind_turbine_subsidies", &Structure::wind_turbine_subsidies, 0);

}

void Structure::_init()
{

}

void Structure::_ready()
{
    object_scale = this->get("scale");

    myCity = this->get_tree()->get_root()->get_node("Main/3Dworld");

    // COUNT UP ALL THE INITIAL VARIABLES

    add_city_counters();
   

}



void Structure::add_city_counters() {
    myCity->set("income", double(myCity->get("income")) + double(this->get("averageWage")));
    myCity->set("carbonEmission", double(myCity->get("carbonEmission")) + double(this->get("CO2Emission")));
    myCity->set("energyDemand", double(myCity->get("energyDemand")) + double(this->get("energyUse")));
    myCity->set("numberOfEmployees", double(myCity->get("numberOfEmployees")) + double(this->get("employment")));
    myCity->set("totalSatisfaction", double(myCity->get("totalSatisfaction")) + satisfaction_weight() * double(this->get("satisfaction")));
    myCity->set("population", int(myCity->get("population")) + int(this->get("numberOfInhabitants")));

    if (this->get_main_type() == String("Housing")) {
        myCity->set("HousingCO2", int(myCity->get("HousingCO2")) + int(this->get("CO2Emission")));
    } else if (this->get_main_type() == String("Energy")) {
        myCity->set("EnergyCO2", int(myCity->get("EnergyCO2")) + int(this->get("CO2Emission")));
    }
    else if(this->get_main_type() == String("Production")) {
        myCity->set("ProductionCO2", int(myCity->get("ProductionCO2")) + int(this->get("CO2Emission")));
    }
    else if(this->get_main_type() == String("Shop")) {
        myCity->set("ShopsCO2", int(myCity->get("ShopsCO2")) + int(this->get("CO2Emission")));
    }

    

}

void Structure::subtract_city_counters() {
    myCity->set("income", double(myCity->get("income")) - double(this->get("averageWage")));
    myCity->set("carbonEmission", double(myCity->get("carbonEmission")) - double(this->get("CO2Emission")));
    myCity->set("energyDemand", double(myCity->get("energyDemand")) - double(this->get("energyUse")));
    myCity->set("numberOfEmployees", double(myCity->get("numberOfEmployees")) -  double(this->get("employment")));
    myCity->set("totalSatisfaction", double(myCity->get("totalSatisfaction")) - satisfaction_weight() * double(this->get("satisfaction")));
    myCity->set("population", int(myCity->get("population")) - int(this->get("numberOfInhabitants")));

    if (this->get_main_type() == String("Housing")) {
        myCity->set("HousingCO2", int(myCity->get("HousingCO2")) - int(this->get("CO2Emission")));
    }
    else if (this->get_main_type() == String("Energy")) {
        myCity->set("EnergyCO2", int(myCity->get("EnergyCO2")) - int(this->get("CO2Emission")));
    }
    else if (this->get_main_type() == String("Production")) {
        myCity->set("ProductionCO2", int(myCity->get("ProductionCO2")) - int(this->get("CO2Emission")));
    }
    else if (this->get_main_type() == String("Shop")) {
        myCity->set("ShopsCO2", int(myCity->get("ShopsCO2")) - int(this->get("CO2Emission")));
    }

}

Vector3 Structure::get_position() {
    return (Vector3)((Spatial*)this)->get_translation();
}

bool Structure::is_other_structure_within_distance(Vector3 other, double distance) {
    Vector3 pos = get_position();
    return  (sqrtf(other[0] * pos[0] + other[1] * pos[1] + other[2] * pos[2]) <= distance);
}

int Structure::satisfaction_weight() {
    String type = this->get_main_type();
    if (type == "Energy") {
        return 10;
    } 
    if (type == "Housing") {
        return 1;
    }
    if (type == "Production") {
        return 5;
    }
    if (type == "Shop") {
        return 3;
    }
    return 1;
}

void Structure::_process(float delta)
{
    
    if (this->get("updatable")) {

        subtract_city_counters();

        this->simulate_step((double)(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("day_tick")) - age); // will run the lowest level simulate step function
       
        add_city_counters();

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
    age += days;
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

        this->get_tree()->get_root()->get_node("Main/3Dworld")->set("time_speed", 0);

        // PLAYER SHOULD NOT BE MOVABLE

        ((Player*)(this->get_tree()->get_root()->get_node("Main/3Dworld/Player")))->set("movable", false);
        this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);

        //this->get_viewport()->warp_mouse(Vector2(get_viewport()->get_size().x / 2, get_viewport()->get_size().y / 2));

        // SET INFO BOX SIZE AND POSITION

        /*
        ((Label*)(this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")))->set("rect_size", Vector2(InfoBoxWidth, (get_viewport()->get_size().y) - 260));

        if (mousePos.x > (get_viewport()->get_size().x) / 2) 
        {
            (this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox"))->set("rect_position", Vector2(60, 200));
        }
        else {
            real_t AdaptedWidth = ((Vector2)(((Label*)(this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")))->get("rect_size"))).x;
            (this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox"))->set("rect_position", Vector2(get_viewport()->get_size().x - AdaptedWidth - 60, 200));
        }
        */

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
        

        //mousePos = Vector2(real_t((double)(get_viewport()->get_size().x) / 2), real_t((double)(get_viewport()->get_size().y) / 2));

        (this->get_tree()->get_root()->get_node("Main/2Dworld/Menus"))->set("position", mousePos);
        

        show_menu();
    }
}

void Structure::show_menu()
{
    this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
    

    
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
    String info = String("INFORMATION - ABOUT THIS STRUCTURE") + String("\n") + String("\n") + String("\n");
    info += "This building is a(n) " + this->get_main_type() + " building. Specifically, it is a " + this->get_object_type() +"." + String("\n") + String("\n");
    info += "This building is " + to_godot_string((int)((((int)this->get("age"))/365))) + " years and " + to_godot_string((int)((((int)this->get("age")) % 365))) + "days old."+ String("\n");
    info += "This building produces " + to_godot_string((int)(this->get("CO2Emission"))) + "metric tonnes of CO2 yearly." + String("\n");
    return  info;
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





