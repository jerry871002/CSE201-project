#include "Restaurant.h"
#include "random"
#include <Area.hpp>
#include <Viewport.hpp>



using namespace godot;


Restaurant::Restaurant()
{
	PanelsOn = true;
	MenuVisible = false;
	Clickable = false;
	income = 100;

	energyUse = 0;


	restaurantStatus = true;
	firstYearShock = false;
	energyUsePerSize = 38;  			//On average 38kWh per square feet
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::normal_distribution <double> mediancost(350000, 100000);  //Median cost of opening restaurant 375K $, including owning the building
	cost = mediancost(gen); 
	
	if (cost < 300000){restaurantType = 1;}
	else if ((300000 <= cost) && (cost <= 400000)){restaurantType = 2;}
	else {restaurantType = 3;}

	//Consider 3 types of restaurant, 1 - small size / fast food type , 2 -Normal Full service resaurant, 3 - Fine dining type 
	switch (restaurantType){
	case 1: {
		std::uniform_real_distribution <double> diningSize1(11, 14); //Average Fast food restaurant has dining space of 11-14 square feet
		diningSize = diningSize1(gen);
 		// maintenance = 0.34;  	Not sure yet
    	satisfaction = 4;	//Smaller sized not very refined restaurant
    	averageWage = 11;		// Euros per hour , Slightly above minimum wage in france
		std::normal_distribution <double> totalemployees1(25, 8);	// 9 out 10 restaurants have less than 50 employees
    	employment = totalemployees1(gen) *  (1 + ((diningSize-12.5)/12.5)); // number of employees influenced by size of restaurent
    	CO2Emission = 825 * ((1 + ((diningSize-13/13)))/10); //on average 301 tons of CO2 per year, hence 825kg per day. 
		buildingTime = 60 ; //On average a casual restaurant takes 12 weeks to set up
		
		break;
	}
	case 2:  {
		std::uniform_real_distribution <double> diningSizeg2(12, 15); //Average Full Service restaurant has dining space of 12-15 square feet
		diningSize = diningSizeg2(gen);
 		// maintenance = 0.34;  	Not sure yet
    	satisfaction = 6;	//Full service normal quality restaurant
    	averageWage = 12.5;		// Euros per hour, Slightly above minimum wage in france
		std::normal_distribution <double> totalemployees2(30, 10);	// 9 out 10 restaurants have less than 50 employees
    	employment = totalemployees2(gen) *  (1 + ((diningSize-13.5)/13.5)); // number of employees influenced by size of restaurent
    	CO2Emission = 825 * ((1 + ((diningSize-13/13)))/10); // need to find more info on this
		buildingTime = 70 ; //On average a casual restaurant takes 14 weeks to set up
		break;
	}
	case 3:{
		std::uniform_real_distribution <double> diningSizeg3(11, 14); //Average Fast food restaurant has dining space of 11-14 square feet
		diningSize = diningSizeg3(gen);
		energyUse = 38;  //On average 38kWh per square feet
 		// maintenance = 0.34;  	Not sure yet
    	satisfaction = 7;	//Luxury aimed restaurant - High quality
    	averageWage = 14;		// Higher than other types as more luxurious 
		std::normal_distribution <double> totalemployees3(35, 7);	// 9 out 10 restaurants have less than 50 employees
    	employment = totalemployees3(gen) *  (1 + ((diningSize-12.5)/12.5)); // number of employees influenced by size of restaurent
    	CO2Emission = 825 * ((1 + ((diningSize-13/13)))/10); // need to find more info on this
		buildingTime = 80 ; //On average a casual restaurant takes 16 weeks to set up

		break;
	}
	}
}

void Restaurant::simulate_step(double days){
	age += days;
	double shock;
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	
	if ((age >= 365) and (firstYearShock == false)){
		std::uniform_real_distribution <double> firstYearShockGen(0,100);
		double shockgen = firstYearShockGen(gen);
		if (shockgen <= 17){
			restaurantStatus = false;		//On average restaurants have a 17% chance of closing in the first year.
		}
		firstYearShock = true; //so that this if statement is only run once, after a full year has passed
	}
	
}

double Restaurant::get_satisfaction(){
	return this->satisfaction;
}

double Restaurant::get_co2emissions(){
	return this->CO2Emission;
}

double Restaurant::get_energyuse(){
	return this->energyUse;
}

double Restaurant::get_environmentalcost(){
	return 0;
}


/*
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
*/

Node* Restaurant::GetPanels()
{
	return get_node("MeshComponents")->get_node("SolarPanels");
}

void Restaurant::_register_methods()
{
	register_method((char*)"_process", &Restaurant::_process);
	register_method((char*)"_input", &Restaurant::_input);
	register_method((char*)"_ready", &Restaurant::_ready);
	register_method((char*)"_on_Area_mouse_entered", &Restaurant::_on_Area_mouse_entered);
	register_method((char*)"_on_Area_mouse_exited", &Restaurant::_on_Area_mouse_exited);
	register_method((char*)"_on_CheckBox_pressed", &Restaurant::_on_CheckBox_pressed);
	register_method((char*)"_on_CheckBox_button_up", &Restaurant::_on_CheckBox_button_up);
	register_method((char*)"_on_CheckBox_button_down", &Restaurant::_on_CheckBox_button_down);
	register_method((char*)"_on_CheckBox_toggled", &Restaurant::_on_CheckBox_toggled);
}



void Restaurant::change_income_by_percent(float factor ) {
	income *= factor;
}

void Restaurant::_init()
{

}

void Restaurant::_process(float delta)
{

}

void Restaurant::_input(InputEvent* e)
{
	Input* i = Input::get_singleton();

	if (i->is_action_pressed("ui_select") && Clickable) {
		PanelsOn = (PanelsOn == false);
		//this->GetPanels()->set("visible", PanelsOn);
		//this->get_child(0)->set("pressed", PanelsOn);
		MenuVisible = (MenuVisible == false);
		//Bthis->get_child(0)->set("rect_position", Vector2(this->get_viewport()->get_mouse_position().x, this->get_viewport()->get_mouse_position().x));
		this->get_child(0)->set("visible", MenuVisible);
	}
}


void Restaurant::_ready()
{
	this->GetPanels()->set("visible", PanelsOn);
	this->get_child(0)->set("pressed", PanelsOn);
	this->get_child(0)->set("text", this->class_name() + ": Select to display solar panels");
	this->get_child(0)->set("visible", MenuVisible);
}

void godot::Restaurant::_on_Area_mouse_entered()
{

	Clickable = true;
	//this->GetPanels()->set("visible", true);

	Input* i = Input::get_singleton();
	// CHANGE MOUSE CURSOR
	i->set_default_cursor_shape(i->CURSOR_POINTING_HAND);
	

}

void godot::Restaurant::_on_Area_mouse_exited()
{
	Clickable = false;
	//this->GetPanels()->set("visible", false);
	Input* i = Input::get_singleton();
	// CHANGE MOUSE CURSOR
	i->set_default_cursor_shape(i->CURSOR_ARROW);
	
}


void godot::Restaurant::_on_CheckBox_pressed() 
{
	
}

void godot::Restaurant::_on_CheckBox_toggled()
{
	PanelsOn = (PanelsOn == false);
	this->GetPanels()->set("visible", PanelsOn);
}


void godot::Restaurant::_on_CheckBox_button_up()
{
	
}

void godot::Restaurant::_on_CheckBox_button_down()
{

}


String godot::Restaurant::class_name()
{
	return "Restaurant";
}