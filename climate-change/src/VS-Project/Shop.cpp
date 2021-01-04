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

    register_property<Shop, double>("panel_probability", &Shop::panel_probability, 0.75);
}

void Shop::_ready() 
{
    this->Structure::_ready();
    this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
}

Shop::Shop()
{
    Clickable = false;
    PanelsOn = false;
    
}

Shop::~Shop()
{

}

/*
void Shop::initialize(int shopTypeInput){
    shopType = shopTypeInput;
    shopStatus = true;
    std::random_device rd;
    std::mt19937 gen(rd());
    switch (shopType) {
        case 0: {
            //Nothing happens as this is the base case. I only implement write it here so that we understand what is happening.
            break;
            }
        case 1:{ //This is when we initialize a restaurant
            energyUsePerSize = 38;  			//On average 38kWh per square feet
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
            break;
        }
        case 2: { //This is a small shop
            satisfaction = 5;       //This is simply an example of code to put in different cases. Will changed later
            break;
        }
        default: { // If none of the cases match - which shouldn't be normal
            std::cout << "Error in initializing Shop, shopType not found" << std::endl;
            break;
        }
    }
}
*/

template<typename T> String to_godot_string(T s)
{
    std::string standardString = std::to_string(s);
    godot::String godotString = godot::String(standardString.c_str());
    return godotString;
}

String Shop::get_object_info()
{
    String info = this->Structure::get_object_info();

    info += "Employement: " + to_godot_string(this->employment) + String("\n");
    if (this->PanelsOn) {
        info += "Panels are displayed" + String("\n") + "Panel age = " + to_godot_string(this->panels_age) + String("\n");
    }
    else {
        info += "Panels are not displayed" + String("\n");
    }
    info += "CO2 Emissions: " + to_godot_string((double)(this->get("CO2Emission")));
    return info;
}

double Shop::get_satisfaction() {
    return this->satisfaction;
}

double Shop::get_co2emissions() {
    //std::cout << "DEBUG: SHOP GET EMISSIONS" << std::endl;
    double factor = 1;
    if (this->PanelsOn) { factor = 0.5; };
    return (double)(this->CO2Emission)*factor;
}

double Shop::get_energyuse() {
    return 0;
}

double Shop::get_environmentalcost() {
    return this->environmentalCost;
}


void Shop::simulate_step(double days) {

    //std::cout << "DEBUG: SHOP SIMULATION CALLED" << std::endl;

    this->Structure::simulate_step(days);

    /*
    Godot::print("DEBUG: PANELS_AGE = ");
    Godot::print(to_godot_string(int(((Node*)this)->get("panels_age"))));
    Godot::print("DEBUG: PANEL_PROBABILITY = ");
    Godot::print(to_godot_string(double(((Node*)this)->get("panel_probability"))));
    */

    if (int(this->panels_age) == 0) {

        // TESTING STUFF LIKELY TO BE REMOVED SOON

        double temp1 = double(1.0 - this->panel_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 
        std::cout << "DEBUG: PANEL AGE = " << std::to_string(this->panels_age) << " AND PANEL PROBABILITY = " << std::to_string(this->panel_probability) << std::endl;
        std::cout << "DEBUG: BEFORE PANEL ADDED IN SIMULATE STEP  r =" << r << " and prob = " << (pow(double(1 - double(this->panel_probability)), double(days / 365.0))) << std::endl; //double(days / 365.0)
        if (r > temp3)
        {
            panels_get_added();
            this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
            std::cout << "DEBUG: PANEL ADDED IN SIMULATE STEP" << std::endl;
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
        std::cout << "DEBUG: PANEL REMOVED" << std::endl;
    }

    //std::cout << "DEBUG: SHOP SIMULATION DONE" << std::endl;

}

void Shop::panels_get_added() {
    PanelsOn = true;
    panels_age = 100;   // set the panels age here ! when they are just built


    // std::cout << "DEBUG: PANEL ADDED IN PANELS GET ADDED" << std::endl;
}






//  #################################   RESTAURANT      ###############################

Restaurant::Restaurant() {
    energyUsePerSize = 38;          //On average 38kWh per square feet 

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
        averageWage = 11;		// Euros per hour , Slightly above minimum wage in france
        std::normal_distribution <double> totalemployees1(25, 8);	// 9 out 10 restaurants have less than 50 employees
        employment = totalemployees1(gen) * (1 + ((diningSize - 12.5) / 12.5)); // number of employees influenced by size of restaurent
        CO2Emission = 825 * ((1 + ((diningSize - 13 / 13))) / 10); //on average 301 tons of CO2 per year, hence 825kg per day. 
        buildingTime = 60; //On average a casual restaurant takes 12 weeks to set up

        break;
    }
    case 2: {
        std::uniform_real_distribution <double> diningSizeg2(12, 15); //Average Full Service restaurant has dining space of 12-15 square feet
        diningSize = diningSizeg2(gen);
        // maintenance = 0.34;  	Not sure yet
        satisfaction = 6;	//Full service normal quality restaurant
        averageWage = 12.5;		// Euros per hour, Slightly above minimum wage in france
        std::normal_distribution <double> totalemployees2(30, 10);	// 9 out 10 restaurants have less than 50 employees
        employment = totalemployees2(gen) * (1 + ((diningSize - 13.5) / 13.5)); // number of employees influenced by size of restaurent
        CO2Emission = 825 * ((1 + ((diningSize - 13 / 13))) / 10); // need to find more info on this
        buildingTime = 70; //On average a casual restaurant takes 14 weeks to set up
        break;
    }
    case 3: {
        std::uniform_real_distribution <double> diningSizeg3(11, 14); //Average Fast food restaurant has dining space of 11-14 square feet
        diningSize = diningSizeg3(gen);
        // maintenance = 0.34;  	Not sure yet
        satisfaction = 7;	//Luxury aimed restaurant - High quality
        averageWage = 14;		// Higher than other types as more luxurious 
        std::normal_distribution <double> totalemployees3(35, 7);	// 9 out 10 restaurants have less than 50 employees
        employment = totalemployees3(gen) * (1 + ((diningSize - 12.5) / 12.5)); // number of employees influenced by size of restaurent
        CO2Emission = 825 * ((1 + ((diningSize - 13 / 13))) / 10); // need to find more info on this
        buildingTime = 80; //On average a casual restaurant takes 16 weeks to set up

        break;
    }
    }
}

Restaurant::~Restaurant() {}

void Restaurant::simulate_step(double days) {
    
    //std::cout << "DEBUG: RESTAURANT SIMULATION CALLED" << std::endl;

    this->Shop::simulate_step(days);
    
    age += days;
    double shock = 0;
    std::random_device rd;
    std::mt19937 gen(rd());

    if ((age >= 365) and (firstYearShock == false)) {
        std::uniform_real_distribution <double> firstYearShockGen(0, 100);
        double shockgen = firstYearShockGen(gen);
        if (shockgen <= 17) {
            shopStatus = false;		//On average restaurants have a 17% chance of closing in the first year.
        }
        firstYearShock = true; //so that this if statement is only run once, after a full year has passed
    }

    //std::cout << "DEBUG: RESTAURANT SIMULATION DONE" << std::endl;

}

double Restaurant::get_energyuse() {
    return (this->energyUsePerSize) * (this->diningSize);
}


// #############    Small Shop          ####################

SmallShop::SmallShop() {}

SmallShop::~SmallShop() {}



// ################   Mall              ####################

Mall::Mall() {}

Mall::~Mall() {}
