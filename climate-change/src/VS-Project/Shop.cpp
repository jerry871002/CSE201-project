#include <iostream>
#include "random"
#include "Shop.h"
#include <string>
#include <core/Godot.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>



using namespace godot;


// ######################     SHOP SUPERCLASS     ##############################


void Shop::_register_methods() 
{
    register_method((char*)"_ready", &Shop::_ready);
    register_method((char*)"_process", &Shop::_process);
    register_property<Shop, double>("panel_probability", &Shop::panel_probability, 0.75);
}

void Shop::_ready() 
{
    this->Structure::_ready();
    this->get_node("MeshComponents/SolarPanels")->set("visible", false);
    this->get_node("MeshComponents/WindTurbine")->set("visible", false);
}

Shop::Shop()
{
    Clickable = false;
    PanelsOn = false;
}

Shop::~Shop()
{

}

void Shop::_process(float delta) {
    this->Structure::_process(delta);
    rot -= delta * turnSpeed * int(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("time_speed"));
    ((Mesh*)this->get_node("MeshComponents/WindTurbine"))->set("rotation_degrees", Vector3(0, -double((180 / 3.1415926535) * rot), 0));
}

template<typename T> String to_godot_string(T s)
{
    std::string standardString = std::to_string(s);
    godot::String godotString = godot::String(standardString.c_str());
    return godotString;
}

String Shop::get_object_info()
{
    String info = this->Structure::get_object_info();

    info += "This building produces " + to_godot_string((int)(this->get("CO2Emission"))) + " metric tonnes of CO2 yearly." + String("\n");
    info += "Employment: " + to_godot_string((int)(this->employment)) + String("\n");
    if (this->PanelsOn) {
        info += "Panels are displayed" + String("\n") + "This building's solar panels are " + to_godot_string(this->panels_age) + " days old." + String("\n");
    }
    else {
        info += "Panels are not displayed" + String("\n");
    }
    info += "SUBSIDY for Panels: " + to_godot_string((int)(this->get("solar_panel_subsidies"))) + String("\n");
    info += "Panels get Added PROBABILITY: " + to_godot_string((double)this->panel_probability) + String("\n");
    if (this->WindTurbineOn) { info += "This building has a rooftop mounted wind turbine."; }
    else {
        info += "If the business owner decides to buy wind turbines, they'd get a government subsidy of " + to_godot_string((int)(this->get("wind_turbine_subsidies"))) + String("\n");
    }
    info += "Satisfaction meter for this building, out of 10: " + to_godot_string((int)(this->get("satisfaction"))) + String("\n");
    
    return info;
}

double Shop::get_satisfaction() {

    double panelsF = 0;
	double roofturbineF = 0;
    if (this->PanelsOn) { panelsF= 2; };
    if (this->WindTurbineOn) { roofturbineF = 1.1; }; //Increase satisfaction if more eco friendly
    

	if (this->satisfaction + panelsF + roofturbineF  > 9.8){
		return 9.8;
	}
	else {
		return this->satisfaction += panelsF + roofturbineF ;
	}

}

double Shop::get_co2emissions() {
    return (double)(this->CO2Emission);
}

double Shop::get_energyuse() {
    double panelsF = 1;
    double turbineF = 1;  
    if (this->PanelsOn) { panelsF = 0.5; };
    if (this->WindTurbineOn) { turbineF = 0.9; };
    return (double)((this->energyUse)*panelsF*turbineF);
}

double Shop::get_environmentalcost() {
    return this->environmentalCost;
}


void Shop::simulate_step(double days) {
    age += days;

    this->Structure::simulate_step(days);

    this->Shop::panel_added_probability();  //This sets the probability of adding solar panels
    this->Shop::windTurbine_added_probability();  //This sets the probability of adding solar panels


    if (int(this->panels_age) == 0) {

        //
        double temp1 = double(1.0 - this->panel_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 
        if (r > temp3)
        {
            PanelsOn = true;
            panels_age = this->solarLifetime;
            this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
            this->get_tree()->get_root()->get_node("Main/3Dworld")->set("budget", (double(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("budget")) - (double)this->get("solar_panel_subsidies"))); // line that takes off from budget the subsidy
        }
        else {}
    }
    else if (int(this->panels_age) > days)
    {
        this->panels_age -= int(days);
    }
    else {
        this->panels_age = 0;
        PanelsOn = false;
        this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
    }

    // Now similar step for wind turbine policy

    if (int(this->windTurbineAge) == 0) {

        //
        double temp1 = double(1.0 - this->windTurbine_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 
        if (r > temp3)
        {
            WindTurbineOn = true;
            windTurbineAge = this->windLifetime;
            this->get_node("MeshComponents/WindTurbine")->set("visible", WindTurbineOn);  //need to have correct meshcomponent
        }
        else {}
    }
    else if (int(this->windTurbineAge) > days)
    {
        this->windTurbineAge -= int(days);
    }
    else {
        this->windTurbineAge = 0;
        WindTurbineOn = false;
        this->get_node("MeshComponents/WindTurbine")->set("visible", WindTurbineOn);
        this->get_tree()->get_root()->get_node("Main/3Dworld")->set("budget", (double(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("budget")) - (double)this->get("wind_turbine_subsidies"))); // line that takes off from budget the subsidy
    }

}

void Shop::panels_get_added() {
    PanelsOn = true;
    panels_age = this->solarLifetime;   // set the panels age here ! when they are just built
    double budget = this->get_tree()->get_root()->get_node("Main/3Dworld")->get("budget");
    this->get_tree()->get_root()->get_node("Main/3Dworld")->set("budget", budget - (double)(this->get("solar_panel_subsidies")));
    // set the panels age here ! when they are just built

}


void Shop::panel_added_probability(){

    //income represents how the economy is doing in general, need to convert it to index between 0-1 
    double panelCost;
    double panel_subsidies = this->get("solar_panel_subsidies"); // input from user of how much are the subsidies
    double income_indexed = 0.5;
    panelCost = this->solarCost - panel_subsidies;  
    if (panelCost < 0) {panelCost = 0;}

    int object_type = this->shopType;
    double initial_investment;  //Give a value between 0-1 to have an idea of the initial investment into the shop and hence the 
    // size of the shop and average wealth. If closer to 1 means more investment and hence more high end and more likely to invest in panels
    switch (object_type)
    {
    case 0:{
        //Nothing will happen
        }
        break;

    case 1:{

        if (this->cost < 300000) {initial_investment = 0.3;}
        else if ((300000 <= this->cost) && (this->cost <= 400000)) {initial_investment = 0.6;}
        else {initial_investment = 0.8;}

        }
        break;
    case 2:{

        if (this->cost < 28000){initial_investment = 0.3;} 
        else if ((28000 <= this->cost) && (this->cost <= 40000)){initial_investment = 0.6;}
        else {initial_investment = 0.8;}

        }
        break;

    case 3:{
        initial_investment = 1 - ((250000000 - (this->cost)) / 250000000);  //if cost very high will get value close to 1. 
        }
        break;

    default:
        break;
    }
    panel_probability = (((this->solarCost - panelCost)/this->solarCost)*50 + ((initial_investment + this->solarSatisfaction/10)/2)*25 + income_indexed*25)/100;

    if(PanelsOn == true){panel_probability = 0;}

}


void Shop::windTurbine_get_added() {
    WindTurbineOn = true;
    windTurbineAge = this->windLifetime;   // set the panels age here ! when they are just built
}

void Shop::windTurbine_added_probability(){
    double windTurbineCost;
    double windTurbine_subsidies = this->get("wind_turbine_subsidies"); // input from user of how much are the subsidies for the wind turbine
    double income_indexed = 0.7;
    windTurbineCost = this->windCost - windTurbine_subsidies;  
    if (windTurbineCost < 0) {windTurbineCost = 0;}

    int object_type = this->shopType;
    double initial_investment;  //Give a value between 0-1 to have an idea of the initial investment into the shop and hence the 
    // size of the shop and average wealth. If closer to 1 means more investment and hence more high end and more likely to invest in panels
    switch (object_type)
    {
    case 0:{
        //Nothing will happen
        }
        break;

    case 1:{

        if (this->cost < 300000) {initial_investment = 0.4;}
        else if ((300000 <= this->cost) && (this->cost <= 400000)) {initial_investment = 0.7;}
        else {initial_investment = 0.8;}

        }
        break;
    case 2:{

        if (this->cost < 28000){initial_investment = 0.4;} 
        else if ((28000 <= this->cost) && (this->cost <= 40000)){initial_investment = 0.7;}
        else {initial_investment = 0.7;}

        }
        break;

    case 3:{
        initial_investment = 1 - ((250000000 - (this->cost)) / 250000000);  //if cost very high will get value close to 1. 
        }
        break;

    default:
        break;
    }

    windTurbine_probability = (((this->windCost - windTurbineCost)/this->windCost)*60 + (initial_investment)*15 + ((income_indexed+this->windSatisfaction/10)/2)*35)/100;

}


//  #################################   RESTAURANT      ###############################


Restaurant::Restaurant() {
    shopType = 1;

    energyUsePerSize = 3000;          //On average 3000kWh per square feet per year
    
    std::random_device rd; 
    std::mt19937 gen(rd());	

    std::normal_distribution <double> mediancost(350000, 100000);  //Median cost of opening restaurant 375K $, including owning the building
    cost = mediancost(gen);

    if (cost < 300000) { restaurantType = 1; }
    else if ((300000 <= cost) && (cost <= 400000)) { restaurantType = 2; }
    else { restaurantType = 3; }

    //Consider 3 types of restaurant, 1 - small size / fast food type , 2 -Normal Full service resaurant, 3 - Fine dining type 
    switch (restaurantType) {
    case 1: {
        std::uniform_real_distribution <double> diningSize1(11, 14); //Average Fast food restaurant has dining space of 11-14 square feet
        diningSize = diningSize1(gen);
        // maintenance = 0.34;  	Not sure yet
        satisfaction = 4;	//Smaller sized not very refined restaurant
        averageWage = 11*8;		// Euros per hour , Slightly above minimum wage in france
        std::normal_distribution <double> totalemployees1(25, 8);	// 9 out 10 restaurants have less than 50 employees
        employment = totalemployees1(gen) * (1 + ((diningSize - 12.5) / 12.5)); // number of employees influenced by size of restaurent
        CO2Emission = 3 * ((1 + ((diningSize - 13 / 13))) / 10); 
        buildingTime = 60; //On average a casual restaurant takes 12 weeks to set up

        break;
    }
    case 2: {
        std::uniform_real_distribution <double> diningSizeg2(12, 15); //Average Full Service restaurant has dining space of 12-15 square feet
        diningSize = diningSizeg2(gen);
        // maintenance = 0.34;  	Not sure yet
        satisfaction = 6;	//Full service normal quality restaurant
        averageWage = 12.5*8;		// Euros per hour, Slightly above minimum wage in france
        std::normal_distribution <double> totalemployees2(30, 10);	// 9 out 10 restaurants have less than 50 employees
        employment = totalemployees2(gen) * (1 + ((diningSize - 13.5) / 13.5)); // number of employees influenced by size of restaurent
        CO2Emission = 3 * ((1 + ((diningSize - 13 / 13))) / 10); 
        buildingTime = 70; //On average a casual restaurant takes 14 weeks to set up
        break;
    }
    case 3: {
        std::uniform_real_distribution <double> diningSizeg3(11, 14); //Average Fast food restaurant has dining space of 11-14 square feet
        diningSize = diningSizeg3(gen);
        // maintenance = 0.34;  	Not sure yet
        satisfaction = 7;	//Luxury aimed restaurant - High quality
        averageWage = 14*8;		// Higher than other types as more luxurious 
        std::normal_distribution <double> totalemployees3(35, 7);	// 9 out 10 restaurants have less than 50 employees
        employment = totalemployees3(gen) * (1 + ((diningSize - 12.5) / 12.5)); // number of employees influenced by size of restaurent
        CO2Emission = 3 * ((1 + ((diningSize - 13 / 13))) / 10); // need to find more info on this
        buildingTime = 80; //On average a casual restaurant takes 16 weeks to set up

        break;
    }
    }

}



Restaurant::~Restaurant() {}

void Restaurant::simulate_step(double days){
    this->Shop::simulate_step(days);

	//double shock;
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	
	if ((age >= 365) and (firstYearShock == false)){
		std::uniform_real_distribution <double> firstYearShockGen(0,100);
		double shockgen = firstYearShockGen(gen);
		if (shockgen <= 17){
			shopStatus = false;		//On average restaurants have a 17% chance of closing in the first year.
		}
		firstYearShock = true; //so that this if statement is only run once, after a full year has passed
	}
	
}

double Restaurant::get_energyuse(){
    double panels = 1;
    double turbine = 1;  
    if (this->PanelsOn) { panels = 0.65; };
    if (this->WindTurbineOn) { turbine = 0.8;};
    return (double)((this->energyUsePerSize)*(this->diningSize)*turbine*panels);

}

// #############    Small Shop          ####################

SmallShop::SmallShop(){
    shopType = 2;

    std::random_device rd; 
	std::mt19937 gen(rd()); 

    std::normal_distribution <double> mediancost(35000, 10000);  //Median cost of opening small retail store around 35K $
    cost = mediancost(gen);

    if (cost < 28000) { smallShopType = 1; }
    else if ((28000 <= cost) && (cost <= 40000)) { smallShopType = 2; }
    else { smallShopType = 3; }
    
    std::uniform_real_distribution <double> averageWageperyear(30000,100000);
    averageWage = averageWageperyear(gen)/365; //gives average wage per day of employees. 

    double energyuseperyear;

    //Consider 3 types of Small Shops, 1 - Micro Business , 2 - Small business, 3 - Medium business
    switch (smallShopType) {
        case 1: {
            std::uniform_real_distribution <double> energyusepeyear1(5000, 15000); // Micro business have an average energy consumption between 5000-15000 KWh per year
            energyuseperyear = energyusepeyear1(gen);
            energyUse = energyuseperyear;
            satisfaction = 6;

            CO2Emission = 2.5;  //these are approximates as specific data is very hard to find
            std::uniform_real_distribution <double> totalemployees1(1, 5);	
            employment = totalemployees1(gen);
        }
        break;
        case 2: {
            std::uniform_real_distribution <double> energyusepeyear2(15000, 30000); // Small business have an average energy consumption between 15000-30000 KWh per year
            energyuseperyear = energyusepeyear2(gen);
            energyUse = energyuseperyear;
            satisfaction = 6;
            CO2Emission = 3;  //these are approximates as specific data is very hard to find
            std::uniform_real_distribution <double> totalemployees2(5, 15);	
            employment = totalemployees2(gen);
            }
            break;
        case 3: {
            std::uniform_real_distribution <double> energyusepeyear3(30000, 45000); // Medium business have an average energy consumption between 30000-45000 KWh per year
            energyuseperyear = energyusepeyear3(gen);
            energyUse = energyuseperyear;
            satisfaction = 6;

            CO2Emission = 3.5;  //these are approximates as specific data is very hard to find
            std::uniform_real_distribution <double> totalemployees3(10,20);	
            employment = totalemployees3(gen);
            }
        break;
    }
}

SmallShop::~SmallShop(){}



void SmallShop::simulate_step(double days){
    this->Shop::simulate_step(days);
}

// ################   Mall              ####################

Mall::Mall(){
    shopType = 3;

    std::random_device rd; 
	std::mt19937 gen(rd()); 

    std::uniform_real_distribution <double> mediancost(25000000, 50000000);  //Median cost of opening medium sized shopping mall, between 50-250 million 
    cost = mediancost(gen);
    
    satisfaction = 7;
    CO2Emission = 50;  //Impossible to find specific data about malls in general, Used an estimate from averages of restaurants and small shops, and energy use of a mall

    buildingTime = 365*3; //Around 3 years, but can vary a lot
    environmentalCost = 0; //No data found for this

    std::uniform_real_distribution <double> energyusepeyeargen1(300000, 500000);
    energyUse = energyusepeyeargen1(gen);

    std::uniform_real_distribution <double> employmentgen1(150, 300);
    employment = employmentgen1(gen);
}

Mall::~Mall(){}

void Mall::simulate_step(double days) {
    this->Shop::simulate_step(days);
}

