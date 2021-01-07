#include "Housing.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>
#include <cstdlib>
#include <ctime>
#include <Viewport.hpp>
#include <SceneTree.hpp>
#include <string>

using namespace godot;

/* For sim team: Can you make a function for double glazing making windows be changed, same for wind turbines. 
Also is there a variable name for no solar panels. Because after 25 years, solar panels are removed automatically
I would like to know how this is translated in the code, because right now I only know how to make them appear.
*/

// yes we can work on this , send us a text if theirs anything specific bothering you in the meanwhile



/// HOUSING SUPER CLASS
/// This is the super class that contains classes for houses and buildings of our city.

Housing::Housing() {

	environmentalCost = 0;

}

Housing::~Housing() {

}


void Housing::_register_methods()
{
	register_method((char*)"_ready", &Housing::_ready);
}

void Housing::_ready()
{
	this->Structure::_ready();
}

void Housing::simulate_step(double days) {

	std::cout << "DEBUG: HOUSING SIMULATION CALLED" << std::endl;
	this->Structure::simulate_step(days); 
};


double Housing::get_max_income() {
    return this->maxIncome;
}

double Housing::get_min_income() {
    return this->minIncome;
}

double Housing::get_solar_panel_age() {
    return this->solarPanelAge;
}

double Housing::get_double_glazing_age() {
    return this->doubleGlazingAge;
}




///	HOUSE CLASS
/* This is the class that represents the houses in the city. There are two typed of houses : 
--> low-level houses: houses with bad insulation, that consume a lot of energy because of that 
--> high-level houses: houses with either solar panels or rooftop wind turbine (one or the other is chosen randomly when building a new house
*/

House::House() {
	//how should i build a house ?

	if (double_glazing() == true) {
        houseType = 2; //High level House
    }

    else {
        houseType = 1; //Low level House
    }

    switch (houseType){
	case 1: { //Low level house
		/*solar_panel() = false;
		double_glazing() = false;
		rooftop_wind_turbines() = false;*/
		srand((int)time(0));
        //minimum wage 53 € per day - get money even on saturday and saturday 
        // i take max to be 333€

        numberOfInhabitants = (rand() % (6) + 1);
        if (numberOfInhabitants >= 2) {
            //have two salaries 
            //houseIncome = (rand() % (333-53)) + 53 + (rand() % (333-53)) + 53;  //random number in [53,333]
			houseIncome = (rand() % (maxIncome-minIncome)) + minIncome + (rand() % (maxIncome-minIncome)) + minIncome;
		}

        else {
            houseIncome = (rand() % (maxIncome-minIncome)) + minIncome;
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
		/*solar_panel() = true;
		double_glazing() = true;
		rooftop_wind_turbines() = true;*/

		srand((int)time(0));
        //minimum wage 53 € per day - get money even on saturday and saturday but assuming working only for 
        // i take max to be 333€
        numberOfInhabitants = (rand() % (6) + 1);
        if (numberOfInhabitants >= 2) {
            //have two salaries 
            houseIncome = (rand() % (maxIncome-minIncome)) + minIncome + (rand() % (maxIncome-minIncome)) + minIncome;
			// houseIncome = (rand() % (333-53)) + 53 + (rand() % (333-53)) + 53;  //random number in [53,333]
        } else {
            houseIncome = (rand() % (maxIncome-minIncome)) + minIncome;
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
	if ((int)(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("day_tick")) % 25 == 0) {
		satisfaction = 0;

	}

	if (solar_panel() == true) {
		solarPanelAge += days; //or should it be + days?
		
	}
	
	if (double_glazing() == true) {
		doubleGlazingAge += days;
	} 

	if (rooftop_wind_turbines() == true) {
		rooftopWindTurbineAge += days;
	}

	/*         
	if (solarPanelAge >= solarLifetime) {
		//Then the solar panels are removed and we have 
		solar_panel() = false;
		solarPanelAge = 0;
	}

	if (doubleGlazingAge >= 18250) {
		//remove windows, they need to be changed - add a message that says that windows need to be changed, becaue can't leave house without windows
		double_glazing() = false;
		doubleGlazingAge = 0;
	} 

	if rooftopWindTurbineAge >= windLifetime {
		rooftop_wind_turbines() = false;
		rooftopWindTurbineAge = 0;
	} 
	
	
	*/


	switch (houseType){
		case 1: {
			if (solar_panel() == true) {
				energyUse -= solarEnergy;
				//maintenance += solarPanelCost;
			}
			
			if (double_glazing() == true) {
				energyUse -= energyUse * 0.25 ; //when having better insulation of windows, you don't have the 25% loss of heat anymore            
				//maintenance += windowCost * windowNumber; //200€ per window, if function adds one window at a time
			}

			if (rooftop_wind_turbines() == true) {
				energyUse -= windEnergy;
			}

			maintenance += 0.1765 * energyUse * days;
			CO2Emission += 0.0065 * energyUse * days; 
		}

		case 2: {
			maintenance += 0.1765 * energyUse * days;
			CO2Emission += 0.0065 * energyUse * days; 
		}

		break;
	}
	
	age += days; // age is an attribute from the Structure class
	
}

bool House::solar_panel() {
	if (rooftop_wind_turbines() == true) {
		//we cannot have solar panels and wind turbines on the same house
		//a message should appear, saying that the solar_panels cannot be installed 
		return false;
		//return PanelsOff
	}

	else {
		if (houseIncome > 25) { //the people that live in the house have enough money to install solar panels so they can 
			solarPanelAge = 0; 
			if (satisfaction < 10) {
				satisfaction += 1;
			}

			return PanelsOn;
		}

		else {
			if (solar_panel_subsidies() == true) {
				//then the household gets financial help and can install the solar_panels
				solarPanelAge = 0; 
				if (satisfaction < 10) {
					satisfaction += 1;
				}

				return PanelsOn;
			}

			else {
				//return PanelsOff;
				return false;
			}
		}
	}
	                            
}

//placeholder must change 

bool House::double_glazing() {
	if (houseIncome > 150) {
		doubleGlazingAge = 0;
		return true;	
	}

	else {
		if (double_glazing_subsidies() == true) {
				//then the household gets financial help
				doubleGlazingAge = 0; 
				if (satisfaction < 10) {
					satisfaction += 1;
				}

				return true;
			}

			else {
				return false;
			}
	}
} 

bool House::rooftop_wind_turbines() {
	if (solar_panel() == true) {
		//we cannot have solar panels and wind turbines on the same house
		//a message should appear, saying that the turbines cannot be installed 
		return false;
		//return PanelsOff
	}

	else {
		if (houseIncome > 280) { //the people that live in the house have enough money to install wind turbines so they can 
			rooftopWindTurbineAge = 0; 
			if (satisfaction < 10) {
				satisfaction += 1;
			}

			return true;
		}

		else {
			if (wind_turbine_subsidies() == true) {
				//then the household gets financial help and can install the wind turbines
				rooftopWindTurbineAge = 0; 
				if (satisfaction < 10) {
					satisfaction += 1;
				}

				return true;
			}

			else {
				//return PanelsOff;
				return false;
			}
		}
	}
}

/// <summary>
///	BUILDING CLASS
/// </summary>

Building::Building() {

	
	if (double_glazing() == true) {
        buildingType = 2; //High level Building
    }

    else {
        buildingType = 1; //Low level Building
    }

    switch (buildingType){
	case 1: { //Low level house
		/*solar_panel() = false;
		rooftop_wind_turbines() = false;
		double_glazing() = false;*/

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
		/*solar_panel() = true;
		double_glazing() = true;
		rooftop_wind_turbines() = true;*/

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

	std::cout << "DEBUG: BUILDING SIMULATION CALLED" << std::endl;

	this->Housing::simulate_step(days);

	/*

	if (solar_panel() == true) {
		solarPanelAge += 1;
	}
			
	//assuming this is a button to change all windows at once i.e. you cannot call this function more than once on this house	
	if (double_glazing() == true) {
		doubleGlazingAge += 1;
		
	}

	if (rooftop_wind_turbines() == true) {
		rooftopWindTurbineAge += 1;
	}
	
	
	if (solarPanelAge >= solarLifetime) {
		//Then the solar panels are removed and we have 
		solar_panel() = false;
		solarPanelAge = 0;
	}

	if (doubleGlazingAge >= 18250) {
		//remove windows, they need to be changed 
		double_glazing() = false;
		doubleGlazingAge = 0;
	}
	
	if (rooftopWindTurbineAge >= windLifetime) {
		rooftop_wind_turbines() = false;
		rooftopWindTurbineAge = 0;
	} 
	
    
	switch (buildingType){
	case 1: { //Low level house
		if (double_glazing() == true) {
			energyUse *= 0.75; //when having better insulation of windows, you don't have the 25% loss of heat anymore
		}

		if (solar_panel() == true) {
			energyUse -= solarEnergy;
		}

		if (rooftop_wind_turbines() == true) {
			energyUse -= windEnergy;
		}

		maintenance += 0.1765 * energyUse * days;
		CO2Emission += 0.0065 * energyUse * days;  

		break;
	}

	case 2:  {
		maintenance += 0.1765 * energyUse * days;
		CO2Emission += 0.0065 * energyUse * days;  
		break;
	}
	
	}
	
	age += days; //age is an attribute from the Structure class
	*/
}

bool Building::solar_panel() {
	if (rooftop_wind_turbines() == true) {
		//we cannot have solar panels and wind turbines on the same house
		//a message should appear, saying that the solar_panels cannot be installed 
		return false;
		//return PanelsOff
	}

	else {
		if (coOwnershipBudget > 25) { //the people that live in the house have enough money to install solar panels so they can 
			solarPanelAge = 0; 
			if (satisfaction < 10) {
				satisfaction += 1;
			}

			return PanelsOn;
		}

		else {
			if (solar_panel_subsidies() == true) {
				//then the household gets financial help and can install the solar_panels
				solarPanelAge = 0; 
				if (satisfaction < 10) {
					satisfaction += 1;
				}

				return PanelsOn;
			}

			else {
				//return PanelsOff;
				return false;
			}
		}
	}

	
}

bool Building::double_glazing() {
	if (coOwnershipBudget > 20) {
		doubleGlazingAge = 0;
		return true;	
	}

	else {
		if (double_glazing_subsidies() == true) {
				//then the household gets financial help
				doubleGlazingAge = 0; 
				if (satisfaction < 10) {
					satisfaction += 1;
				}

				return true;
			}

			else {
				return false;
			}
	}

}


bool Building::rooftop_wind_turbines() {
	if (solar_panel() == true) {
		//we cannot have solar panels and wind turbines on the same house
		//a message should appear, saying that the turbines cannot be installed 
		return false;
		//return PanelsOff
	}

	else {
		if (coOwnershipBudget > 50) { //the people that live in the house have enough money to install wind turbines so they can 
			rooftopWindTurbineAge = 0; 
			if (satisfaction < 10) {
				satisfaction += 1;
			}

			return true;
		}

		else {
			if (wind_turbine_subsidies() == true) {
				//then the household gets financial help and can install the wind turbines
				rooftopWindTurbineAge = 0; 
				if (satisfaction < 10) {
					satisfaction += 1;
				}

				return true;
			}

			else {
				//return PanelsOff;
				return false;
			}
		}
	}
}

//INFORMATION DISPLAY 

template<typename T> String to_godot_string(T s)
{
	std::string standardString = std::to_string(s);
	godot::String godotString = godot::String(standardString.c_str());
	return godotString;
}

String Housing::get_object_info()
{
	String info = this->Structure::get_object_info();

	info += "Age of the building in days: " + to_godot_string((double)(this->get("age"))) + String("\n");
	info += "CO2 Emissions: " + to_godot_string((double)(this->get("CO2Emission"))) + String("\n");
	info += "Energy used by the building in kWh: " + to_godot_string(this->energyUse) + String("\n");
	info += "Satisfaction meter, out of 10: " + to_godot_string((int)this->get("satisfaction")) + String("\n");
	return info;
}

