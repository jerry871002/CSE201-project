
#include <iostream>
#include "random"
#include "Shop.h"

#include <core/Godot.hpp>



using namespace godot;
// ######################     SHOP     ##############################

Shop::Shop(){}

Shop::~Shop() {}
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

double Shop::get_satisfaction(){
    return this->satisfaction;
}

double Shop::get_co2emissions(){
    return this->satisfaction;
}

double Shop::get_energyuse(){
    return 0; 
}

double Shop::get_environmentalcost(){
    return this->environmentalCost;
}

void Shop::simulate_step(double days){

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

void Restaurant::simulate_step(double days){
	age += days;
	double shock;
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
	return (this->energyUsePerSize)*(this->diningSize);
}

// #############    Small Shop          ####################

SmallShop::SmallShop(){}

SmallShop::~SmallShop(){}



// ################   Mall              ####################

Mall::Mall(){}

Mall::~Mall(){}