#include "Housing.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>
#include <cstdlib>
#include <ctime>

using namespace godot;

/// <summary>
/// HOUSING SUPER CLASS
/// </summary>

Housing::Housing() {

}

Housing::~Housing() {

}

double Housing::get_satisfaction() {
    return this->satisfaction;
}

double Housing::get_co2emissions() {
    return this->CO2Emission;
}

double Housing::get_energyuse() {
    return this->energyUse;
}

double Housing::get_environmentalcost() {
    return 0;
}


/// <summary>
///	HOUSE CLASS
/// </summary>


House::House() {
	if (solar_panel() == true and double_glazing() == true) {
        houseType = 2; //High level House
    }

    else {
        houseType = 1; //Low level House
    }

    switch (houseType){
	case 1: { //Low level house
		srand((int)time(0));
        //minimum wage 53 € per day - get money even on saturday and saturday 
        // i take max to be 333€

        numberOfInhabitants = (rand() % (6) + 1);
        if (numberOfInhabitants >= 2) {
            //have two salaries 
            houseIncome = (rand() % (333-53)) + 53 + (rand() % (333-53)) + 53;  //random number in [53,333]
        }

        else {
            houseIncome = (rand() % (333-53)) + 53;
        }
        
        
        //attributes from structure class
        cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
        energyUse= 68.49; //25000kWh per year i.e. 13.69 kWh per day (from heating and all )
        maintenance = 0.1765; //cost in euros per kWh
        CO2Emission = 0.0065; //6.5g per kWh
        buildingTime = 140; //in average, building a house takes about 140 days
        satisfaction = 3; //assuming we are on a scale from 0 to 10
        //attributes special to this class
        windowNumber = 5; 
        age = 0; // set the age of the house at 0 i.e. it was just 
            
		break;
	}
	case 2:  {
		srand((int)time(0));
        //minimum wage 53 € per day - get money even on saturday and saturday but assuming working only for 
        // i take max to be 333€
        numberOfInhabitants = (rand() % (6) + 1);
        if (numberOfInhabitants >= 2) {
            //have two salaries 
            houseIncome = (rand() % (333-53)) + 53 + (rand() % (333-53)) + 53;  //random number in [53,333]
        } else {
            houseIncome = (rand() % (333-53)) + 53;
        }
        //attributes from structure class
        cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
        energyUse = 54.79; //20000kWh per year i.e. 54.79 kWh per day (from heating and all )
        maintenance = 0.1765; //cost in euros per kWh
        CO2Emission = 0.0065; //6.5g per kWh
        buildingTime = 140; //in average, building a house takes about 140 days
        satisfaction = 10; //assuming we are on a scale from 0 to 10
        age = 0;
		break;
	}
	
	}
}

House::~House() {

}

void House::simulate_step(double days) {
	if (houseType == 1) {
        if (solar_panel() == true) {
			maintenance += solarPanelCost;
		}
		if (satisfaction < 10) { //so that we do not end up with a satisfaction above 10
			satisfaction += 1;
		}
	

	//assuming this is a button to change all windows at once i.e. you cannot call this function more than once on this house	
	if (double_glazing() == true) {
		energyUse *= 0.75 ; //when having better insulation of windows, you don't have the 25% loss of heat anymore
		maintenance += windowCost * windowNumber; //200€ per window, if function adds one window at a time
		}
		if (satisfaction < 10) { //so that we do not end up with a satisfaction above 10
			satisfaction += 2;
		}
	} 

	maintenance += 0.1765 * energyUse * days;
	CO2Emission += 0.0065 * energyUse * days;  
	age += days; //age is an attribute from the Structure class
	
}

//For the interface team
bool House::solar_panel() {
	if (houseIncome > 200) {
		return PanelsOn;	
	}

	// else {
	// 	//check if policy from state is giving money
	// 	//in which case you can add solar panels
	// 	//otherwise 
		
	// }
	
}

bool House::double_glazing() {
	// if (houseIncome > 100) {
	// 	return true;	
	// }
	
	// placeholder must change
	return false;
}

/// <summary>
///	BUILDING CLASS
/// </summary>

Building::Building() {
	if (solar_panel() == true and double_glazing() == true) {
        buildingType = 2; //High level Building
    }

    else {
        buildingType = 1; //Low level Building
    }

    switch (buildingType){
	case 1: { //Low level house
		srand((int)time(0));
		//5 appartments pay between 200-400 € per month
		coOwnershipBudget = (rand() % (66-33) + 33);
		
		cost = 1000000; //counting price of lot + cost of workforce + cost of all materials used
		energyUse = 365; // 266.25 kWh/m^2 per year which gives 133125 kWh per year i.e 365 kwH per day
		maintenance = 0.1765; //cost in euros per kWh
		CO2Emission = 0.0065; //6.5g per kWh
		buildingTime = 450; //Time it takes to build an appartment building is about 15 months 
		satisfaction = 3;
		windowNumber = 30;
		age = 0;
		break;
	}

	case 2:  {
		//We consider a 500 m^2 building 
		srand((int)time(0));
		//5 appartments pay between 300-500 € per month
		coOwnershipBudget = (rand() % (83-50) + 50);

		cost = 1000000; //counting price of lot + cost of workforce + cost of all materials used
		energyUse = 292; // 213 kWh/m^2 per year which gives 106500 kWh per year i.e 292 kwH per day
		maintenance = 0.1765; //cost in euros per kWh
		CO2Emission = 0.0065; //6.5g per kWh
		buildingTime = 450; //Time it takes to build an appartment building is about 15 months 
		satisfaction = 7;
		age = 0;
		break;
	}
	
	}
}

Building::~Building() {

}

void Building::simulate_step(double days) {
	if (buildingType == 1) { // if it doesn't already have solar panels because it is high level 
        if (solar_panel() == true) {
			maintenance += solarPanelCost;
		}
			
		if (satisfaction < 10) { //so that we do not end up with a satisfaction above 10
			satisfaction += 1;
		}
	

		//assuming this is a button to change all windows at once i.e. you cannot call this function more than once on this house	
		if (double_glazing() == true) {
			energyUse *= 0.75 ; //when having better insulation of windows, you don't have the 25% loss of heat anymore
			maintenance += windowCost * windowNumber; //200€ per window, if function adds one window at a time
		}
		if (satisfaction < 10) { //so that we do not end up with a satisfaction above 10
			satisfaction += 2;
		}
	}
    
	maintenance += 0.1765 * energyUse * days;
	CO2Emission += 0.0065 * energyUse * days;  
	age += days; //age is an attribute from the Structure class
	
}

bool Building::solar_panel() {
	if (coOwnershipBudget > 50) {
		return PanelsOn;	
	}

	// else {
	// 	//check if policy from state is giving money
	// 	//in which case you can add solar panels
	// 	//otherwise 
		
	// }

	return PanelsOn;
}

bool Building::double_glazing() {
	// if (coOwnershipBudget > 40) {
	// 	return true;	
	// }


	// placeholder must change
	return false;

}

