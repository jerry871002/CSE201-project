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
	info += "Energy used by the building in kWh: " + to_godot_string((double)(this->get("energyUse"))) + String("\n");
	info += "Satisfaction meter, out of 10: " + to_godot_string((int)this->get("satisfaction")) + String("\n");
	return info;
}

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
	this->get_node("MeshComponents/SolarPanels")->set("visible", false);
	this->get_node("MeshComponents/WindTurbine")->set("visible", false);
}

void Housing::simulate_step(double days) {

	std::cout << "DEBUG: HOUSING SIMULATION CALLED" << std::endl;
	this->Structure::simulate_step(days); 
	this->Housing::panel_added_probability();	
	this->Housing::double_glazing_added_probability();
	this->Housing::roof_wind_turbines_added_probability();

	if (int(this->solarPanelAge) == 0) {

        //
        double temp1 = double(1.0 - this->panel_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 
        std::cout << "DEBUG: PANEL AGE = " << std::to_string(this->solarPanelAge) << " AND PANEL PROBABILITY = " << std::to_string(this->panel_probability) << std::endl;
        std::cout << "DEBUG: BEFORE PANEL ADDED IN SIMULATE STEP  r =" << r << " and prob = " << (pow(double(1 - double(this->panel_probability)), double(days / 365.0))) << std::endl; //double(days / 365.0)
        if (r > temp3)
        {
            PanelsOn = true;
            solarPanelAge = 100;
            this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
            std::cout << "DEBUG: PANEL ADDED IN SIMULATE STEP" << std::endl;
        }
        else {}
    }
    else if (int(this->solarPanelAge) > days)
    {
        this->solarPanelAge -= int(days);
    }
    else {
        this->solarPanelAge = 0;
        PanelsOn = false;
        this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
        std::cout << "DEBUG: PANEL REMOVED" << std::endl;
    }

	if (int(this->doubleGlazingAge) == 0) {

        //
        double temp1 = double(1.0 - this->double_glazing_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 
        std::cout << "DEBUG: PANEL AGE = " << std::to_string(this->doubleGlazingAge) << " AND PANEL PROBABILITY = " << std::to_string(this->double_glazing_probability) << std::endl;
        std::cout << "DEBUG: BEFORE PANEL ADDED IN SIMULATE STEP  r =" << r << " and prob = " << (pow(double(1 - double(this->double_glazing_probability)), double(days / 365.0))) << std::endl; //double(days / 365.0)
        if (r > temp3)
        {
            doubleGlazingOn = true;
            doubleGlazingAge = 100;
            
            std::cout << "DEBUG: PANEL ADDED IN SIMULATE STEP" << std::endl;
        }
        else {}
    }
    else if (int(this->doubleGlazingAge) > days) {
        this->doubleGlazingAge -= int(days);
    }
    else {
        this->doubleGlazingAge = 0;
        doubleGlazingOn = false;
        this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
        std::cout << "DEBUG: PANEL REMOVED" << std::endl;
    }

	if (int(this->rooftopWindTurbineAge) == 0) {

        //
        double temp1 = double(1.0 - this->roof_wind_turbines_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 
        std::cout << "DEBUG: PANEL AGE = " << std::to_string(this->rooftopWindTurbineAge) << " AND PANEL PROBABILITY = " << std::to_string(this->roof_wind_turbines_probability) << std::endl;
        std::cout << "DEBUG: BEFORE PANEL ADDED IN SIMULATE STEP  r =" << r << " and prob = " << (pow(double(1 - double(this->roof_wind_turbines_probability)), double(days / 365.0))) << std::endl; //double(days / 365.0)
        if (r > temp3)
        {
            rooftopWindTurbineOn = true;
            rooftopWindTurbineAge = 100;
            this->get_node("MeshComponents/WindTurbine")->set("visible", rooftopWindTurbineOn);
            std::cout << "DEBUG: PANEL ADDED IN SIMULATE STEP" << std::endl;
        }
        else {}
    }
    else if (int(this->rooftopWindTurbineAge) > days)
    {
        this->rooftopWindTurbineAge -= int(days);
    }
    else {
        this->rooftopWindTurbineAge = 0;
        rooftopWindTurbineOn = false;
        this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
        std::cout << "DEBUG: PANEL REMOVED" << std::endl;
    } 

	age += days; 
	if (this->PanelsOn) {
		this->solarPanelAge += days;
	}

	if (this->doubleGlazingOn) {
		this->doubleGlazingAge += days;
	}
	
	if (this->rooftopWindTurbineOn) {
		this->rooftopWindTurbineAge += days;
	}

	if (this->solarPanelAge >= solarLifetime) {
		//Then the solar panels are removed and we have 
		this->PanelsOn = false;
		this->solarPanelAge = 0;
	}

	if (this->doubleGlazingAge >= 18250) {
		this->doubleGlazingOn = false;
		this->doubleGlazingAge = 0;
	} 

	if (this->rooftopWindTurbineAge >= windLifetime) {
		this->rooftopWindTurbineOn = false;
		this->rooftopWindTurbineAge = 0;
	} 
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


void Housing::panel_added_probability(){
	double panelCost;
    double panel_subsidies = this->get("solar_panel_subsidies_housing"); // input from user of how much are the subsidies
    // double income_indexed = 0.5;
    panelCost = this->solarCost - panel_subsidies;  
    if (panelCost < 0) {
		panelCost = 0; //cannot have a negative solar panel cost
	}
	double housingIncomeIndexed = 1 - ((this->maxIncome - this->housingIncome) / this->maxIncome);

	panel_probability = (((this->solarCost - panelCost)/this->solarCost)*50 + (this->solarSatisfaction/10)*15 + housingIncomeIndexed*35)/100;

	if (PanelsOn == true) {
		panel_probability = 0;
	}

	else if (rooftopWindTurbineOn == true) {
		panel_probability = 0;
	}
}

void Housing::double_glazing_added_probability(){
	double doubleGlazingCost;
    double glazing_subsidies = this->get("double_glazing_subsidies"); // input from user of how much are the subsidies
    // double income_indexed = 0.5;
	double housingIncomeIndexed = 1 - ((this->maxIncome - this->housingIncome) / this->maxIncome);
    doubleGlazingCost = windowCost * this->windowNumber - double_glazing_subsidies;

    if (doubleGlazingCost < 0) {
		doubleGlazingCost = 0; //cannot have a negative solar panel cost
	}

	double_glazing_probability = (((windowCost - doubleGlazingCost)/windowCost)*50 + (this->doubleGlazingSatisfaction/10)*15 + housingIncomeIndexed*35)/100;

	if (doubleGlazingOn == true) {
		double_glazing_probability = 0;
	}
}

void Housing::roof_wind_turbines_added_probability(){
	double turbineCost;
    double wind_subsidies = this->get("wind_turbine_subsidies"); // input from user of how much are the subsidies
    // double income_indexed = 0.5;
	double housingIncomeIndexed = 1 - ((this->maxIncome - this->housingIncome) / this->maxIncome);
    turbineCost = this->windCost - wind_turbine_subsidies;  
    if (turbineCost < 0) {
		turbineCost = 0; //cannot have a negative solar panel cost
	}

	roof_wind_turbines_probability = (((this->windCost - turbineCost)/this->windCost)*50 + (this->windSatisfaction/10)*15 + housingIncomeIndexed*35)/100;

	if (rooftopWindTurbineOn == true) {
		roof_wind_turbines_probability = 0;
	}
}


///	HOUSE CLASS
/* This is the class that represents the houses in the city. There are two typed of houses : 
--> low-level houses: houses with bad insulation, that consume a lot of energy because of that 
--> high-level houses: houses with either solar panels or rooftop wind turbine (one or the other is chosen randomly when building a new house
*/

double House::get_co2emissions() {
    return 0.0065 * this->energyUse;
}

double House::get_energyuse() {
	double panelsF = 1;
	double turbineF = 1;
	double glazingF = 1;
	if (this->PanelsOn) {
			panelsF = 0.6;
		}

	if (this->rooftopWindTurbineOn) {
			turbineF = 0.9;
		}
	
	if (this->houseType == 1) { // If its a low level house then double glazing decrease energyuse 
			if (this->doubleGlazingOn) {
				glazingF = 0.75; //when having better insulation of windows, you don't have the 25% loss of heat anymore            
			}
		}
	

    return (double)(this->energyUse)*panelsF*turbineF*glazingF;
}

double House::get_environmentalcost() {
    return this->environmentalCost;
}

House::House() {

	maxIncome=333; 
	minIncome=53; 
	
    switch (houseType){
	case 1: { //Low level house
	//definition of low level house, has no solar panels, no double glazing and no wind turbines on roof 
		PanelsOn = false;
		doubleGlazingOn = false;
		rooftopWindTurbineOn = false;


		srand((int)time(0));
        //minimum wage 53 € per day - get money even on saturday and saturday 
        // i take max to be 333€

        numberOfInhabitants = (rand() % (6) + 1);
        if (numberOfInhabitants >= 2) {
            //have two salaries 
			housingIncome = (rand() % (maxIncome-minIncome)) + minIncome + (rand() % (maxIncome-minIncome)) + minIncome;
		}

        else {
            housingIncome = (rand() % (maxIncome-minIncome)) + minIncome;
        }
        
        
        //Attributes for a low level house 
        cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
        energyUse= 68.49; //25000kWh per year i.e. 13.69 kWh per day (from heating and all )
        maintenance = 0.1765; //cost in euros per kWh
        CO2Emission = 0.0065; //6.5g per kWh
        buildingTime = 140; //in average, building a house takes about 140 days
        satisfaction = 3; //assuming we are on a scale from 0 to 10

        //attributes special to this class
        windowNumber = 5; 
        age = 0; // set the age of the house at 0 
            
		break;
	}

	case 2:  { // High Level House 
		//has properties : double glazing 

		PanelsOn = false;
		doubleGlazingOn = true;
		rooftopWindTurbineOn = false;

		srand((int)time(0));
        numberOfInhabitants = (rand() % (6) + 1);
        if (numberOfInhabitants >= 2) {
            housingIncome = (rand() % (maxIncome-minIncome)) + minIncome + (rand() % (maxIncome-minIncome)) + minIncome;
		
        }
		
		else {
            housingIncome = (rand() % (maxIncome-minIncome)) + minIncome;
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

	std::cout << "DEBUG: BUILDING SIMULATION CALLED" << std::endl;

	this->Housing::simulate_step(days);
	
	// if ((int)(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("day_tick")) % 25 == 0) {
	// 	satisfaction = 0;

	// }

	maintenance = 0.1765 * energyUse * days;
	CO2Emission = 0.0065 * energyUse * days; 

	/*switch (houseType){
		

		case 1: { // If its a low level house then added solar panels decrease energyuse 
			if (PanelOn) {
				energyUse -= solarEnergy;
			}
			
			if (doubleGlazingOn) {
				energyUse -= energyUse * 0.25 ; //when having better insulation of windows, you don't have the 25% loss of heat anymore            
			}

			if (rooftopWindTurbineOn) {
				energyUse -= windEnergy;
			}

			maintenance += 0.1765 * energyUse * days;
			CO2Emission += 0.0065 * energyUse * days; 
		}

		case 2: { //If it's a high level house, no improvements can be made, so simulate step is as follows :
			maintenance += 0.1765 * energyUse * days;
			CO2Emission += 0.0065 * energyUse * days; 
		}

		break;
	}*/

}





/*
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
			/*
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

*/











/// <summary>
///	BUILDING CLASS
/// </summary>

double Building::get_co2emissions() {
    return 0.0065 * this->energyUse;
}

double Building::get_energyuse() {
	double panelsF = 1;
	double turbineF = 1;
	double glazingF = 1;
	if (this->PanelsOn) {
			panelsF = 0.6;
		}

	if (this->rooftopWindTurbineOn) {
			turbineF = 0.9;
		}
	
	if (this->buildingType == 1) { // If its a low level house then double glazing decrease energyuse 
			if (this->doubleGlazingOn) {
				glazingF = 0.75; //when having better insulation of windows, you don't have the 25% loss of heat anymore            
			}
		}
	

    return (double)(this->energyUse)*panelsF*turbineF*glazingF;
}

	


double Building::get_environmentalcost() {
    return this->environmentalCost;
}



Building::Building() {
    switch (buildingType){
	case 1: { //Low level house
		doubleGlazingOn = false;
		rooftopWindTurbineOn = false;
		PanelsOn = false;

		srand((int)time(0));
		//5 appartments pay between 200-400 € per month, this is coownership budget
		housingIncome = (rand() % (66-33) + 33); 
		
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
		doubleGlazingOn = true;
		rooftopWindTurbineOn = false;
		PanelsOn = false;

		//We consider a 500 m^2 building 
		srand((int)time(0));
		//5 appartments pay between 300-500 € per month
		housingIncome = (rand() % (83-50) + 50);

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

	maintenance = 0.1765 * energyUse * days;
	CO2Emission = 0.0065 * energyUse * days;  

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

/*bool Building::solar_panel() {
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
			/*
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
*/




// Housing::Housing() {

// 	environmentalCost = 0;

// }

// Housing::~Housing() {

// }


// void Housing::_register_methods()
// {
// 	register_method((char*)"_ready", &Housing::_ready);
// }

// void Housing::_ready()
// {
// 	this->Structure::_ready();
// }

// void Housing::simulate_step(double days) {

// 	std::cout << "DEBUG: HOUSING SIMULATION CALLED" << std::endl;
// 	this->Structure::simulate_step(days); 
// 	age += days; // age is an attribute from the Structure class
// };


// double Housing::get_max_income() {
//     return this->maxIncome;
// }

// double Housing::get_min_income() {
//     return this->minIncome;
// }

// double Housing::get_solar_panel_age() {
//     return this->solarPanelAge;
// }

// double Housing::get_double_glazing_age() {
//     return this->doubleGlazingAge;
// }




// ///	HOUSE CLASS
// /* This is the class that represents the houses in the city. There are two typed of houses : 
// --> low-level houses: houses with bad insulation, that consume a lot of energy because of that 
// --> high-level houses: houses with either solar panels or rooftop wind turbine (one or the other is chosen randomly when building a new house
// */

// House::House() {
// 	//how should i build a house ?

// 	if (double_glazing() == true) {
//         houseType = 2; //High level House
//     }

//     else {
//         houseType = 1; //Low level House
//     }

//     switch (houseType){
// 	case 1: { //Low level house
// 		/*solar_panel() = false;
// 		double_glazing() = false;
// 		rooftop_wind_turbines() = false;*/
// 		srand((int)time(0));
//         //minimum wage 53 € per day - get money even on saturday and saturday 
//         // i take max to be 333€

//         numberOfInhabitants = (rand() % (6) + 1);
//         if (numberOfInhabitants >= 2) {
//             //have two salaries 
//             //houseIncome = (rand() % (333-53)) + 53 + (rand() % (333-53)) + 53;  //random number in [53,333]
// 			houseIncome = (rand() % (maxIncome-minIncome)) + minIncome + (rand() % (maxIncome-minIncome)) + minIncome;
// 		}

//         else {
//             houseIncome = (rand() % (maxIncome-minIncome)) + minIncome;
//         }
        
        
//         //attributes from structure class
//         cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
//         energyUse= 68.49; //25000kWh per year i.e. 13.69 kWh per day (from heating and all )
//         maintenance = 0.1765; //cost in euros per kWh
//         CO2Emission = 0.0065; //6.5g per kWh
//         buildingTime = 140; //in average, building a house takes about 140 days
//         satisfaction = 3; //assuming we are on a scale from 0 to 10
//         //attributes special to this class
//         windowNumber = 5; 
//         age = 0; // set the age of the house at 0 i.e. it was just 
            
// 		break;
// 	}
// 	case 2:  {
// 		/*solar_panel() = true;
// 		double_glazing() = true;
// 		rooftop_wind_turbines() = true;*/

// 		srand((int)time(0));
//         //minimum wage 53 € per day - get money even on saturday and saturday but assuming working only for 
//         // i take max to be 333€
//         numberOfInhabitants = (rand() % (6) + 1);
//         if (numberOfInhabitants >= 2) {
//             //have two salaries 
//             houseIncome = (rand() % (maxIncome-minIncome)) + minIncome + (rand() % (maxIncome-minIncome)) + minIncome;
// 			// houseIncome = (rand() % (333-53)) + 53 + (rand() % (333-53)) + 53;  //random number in [53,333]
//         } else {
//             houseIncome = (rand() % (maxIncome-minIncome)) + minIncome;
//         }
//         //attributes from structure class
//         cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
//         energyUse = 54.79; //20000kWh per year i.e. 54.79 kWh per day (from heating and all )
//         maintenance = 0.1765; //cost in euros per kWh
//         CO2Emission = 0.0065; //6.5g per kWh
//         buildingTime = 140; //in average, building a house takes about 140 days
//         satisfaction = 10; //assuming we are on a scale from 0 to 10
//         age = 0;
// 		break;
// 	}
// 	}
// }

// House::~House() {

// }

// void House::simulate_step(double days) {

// 	std::cout << "DEBUG: BUILDING SIMULATION CALLED" << std::endl;

// 	this->Housing::simulate_step(days);

// 	/*

// 	if ((int)(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("day_tick")) % 25 == 0) {
// 		satisfaction = 0;

// 	}

// 	if (solar_panel() == true) {
// 		solarPanelAge += days; //or should it be + days?
		
// 	}
	
// 	if (double_glazing() == true) {
// 		doubleGlazingAge += days;
// 	} 

// 	if (rooftop_wind_turbines() == true) {
// 		rooftopWindTurbineAge += days;
// 	}

	
// 	if (solarPanelAge >= solarLifetime) {
// 		//Then the solar panels are removed and we have 
// 		solar_panel() = false;
// 		solarPanelAge = 0;
// 	}

// 	if (doubleGlazingAge >= 18250) {
// 		//remove windows, they need to be changed - add a message that says that windows need to be changed, becaue can't leave house without windows
// 		double_glazing() = false;
// 		doubleGlazingAge = 0;
// 	} 

// 	if rooftopWindTurbineAge >= windLifetime {
// 		rooftop_wind_turbines() = false;
// 		rooftopWindTurbineAge = 0;
// 	} 
	
	
	
// 	*/

// 	std::cout << "DEBUG HOUSE 0" << std::endl;


// 	switch (houseType){
// 		case 1: {

// 			std::cout << "DEBUG HOUSE 1" << std::endl;
// 			if (solar_panel() == true) {
// 				energyUse -= solarEnergy;
// 				//maintenance += solarPanelCost;
// 			}
			
// 			if (double_glazing() == true) {
// 				energyUse -= energyUse * 0.25 ; //when having better insulation of windows, you don't have the 25% loss of heat anymore            
// 				//maintenance += windowCost * windowNumber; //200€ per window, if function adds one window at a time
// 			}

// 			if (rooftop_wind_turbines() == true) {
// 				energyUse -= windEnergy;
// 			}

// 			maintenance += 0.1765 * energyUse * days;
// 			CO2Emission += 0.0065 * energyUse * days; 
// 		}

// 		case 2: {

// 			std::cout << "DEBUG HOUSE 2" << std::endl;
// 			maintenance += 0.1765 * energyUse * days;
// 			CO2Emission += 0.0065 * energyUse * days; 
// 		}

// 		break;
// 	}
	
	

	
	

// }

// bool House::solar_panel() {
// 	/*
// 	if (rooftop_wind_turbines() == true) {
// 		//we cannot have solar panels and wind turbines on the same house
// 		//a message should appear, saying that the solar_panels cannot be installed 
// 		return false;
// 		//return PanelsOff
// 	}
// 	*/

// 	//else {
// 		std::cout << "DEBUG HOUSE 10" << std::endl;
// 		if (houseIncome > 25) { //the people that live in the house have enough money to install solar panels so they can 
// 			solarPanelAge = 0; 
// 			std::cout << "DEBUG HOUSE 20" << std::endl;
// 			if (satisfaction < 10) {
// 				satisfaction += 1;
// 			}
// 			std::cout << "DEBUG HOUSE 30" << std::endl;
// 			return PanelsOn;
// 		}

// 		else {
// 			std::cout << "DEBUG HOUSE 40" << std::endl;
// 			/*
// 			if (solar_panel_subsidies() == true) {
// 				//then the household gets financial help and can install the solar_panels
// 				solarPanelAge = 0; 
// 				if (satisfaction < 10) {
// 					satisfaction += 1;
// 				}

// 				return PanelsOn;
// 			}

// 			else {
// 				//return PanelsOff;
// 				return false;
// 			}
// 			*/
// 		}
// 	//}
	                            
// }

// //placeholder must change 

// bool House::double_glazing() {
// 	if (houseIncome > 150) {
// 		doubleGlazingAge = 0;
// 		return true;	
// 	}

// 	else {
// 		if (double_glazing_subsidies() == true) {
// 				//then the household gets financial help
// 				doubleGlazingAge = 0; 
// 				if (satisfaction < 10) {
// 					satisfaction += 1;
// 				}

// 				return true;
// 			}

// 			else {
// 				return false;
// 			}
// 	}
// } 

// bool House::rooftop_wind_turbines() {
// 	/*
// 	if (solar_panel() == true) {
// 		//we cannot have solar panels and wind turbines on the same house
// 		//a message should appear, saying that the turbines cannot be installed 
// 		return false;
// 		//return PanelsOff
// 	}
// 	*/

// 	//else {
// 		if (houseIncome > 280) { //the people that live in the house have enough money to install wind turbines so they can 
// 			rooftopWindTurbineAge = 0; 
// 			if (satisfaction < 10) {
// 				satisfaction += 1;
// 			}

// 			return true;
// 		}

// 		else {
// 			if (wind_turbine_subsidies() == true) {
// 				//then the household gets financial help and can install the wind turbines
// 				rooftopWindTurbineAge = 0; 
// 				if (satisfaction < 10) {
// 					satisfaction += 1;
// 				}

// 				return true;
// 			}

// 			else {
// 				//return PanelsOff;
// 				return false;
// 			}
// 		}
// 	//}
// }

// /// <summary>
// ///	BUILDING CLASS
// /// </summary>

// Building::Building() {

	
// 	if (double_glazing() == true) {
//         buildingType = 2; //High level Building
//     }

//     else {
//         buildingType = 1; //Low level Building
//     }

//     switch (buildingType){
// 	case 1: { //Low level house
// 		/*solar_panel() = false;
// 		rooftop_wind_turbines() = false;
// 		double_glazing() = false;*/

// 		srand((int)time(0));
// 		//5 appartments pay between 200-400 € per month
// 		coOwnershipBudget = (rand() % (66-33) + 33);
		
// 		cost = 1000000; //counting price of lot + cost of workforce + cost of all materials used
// 		energyUse = 365; // 266.25 kWh/m^2 per year which gives 133125 kWh per year i.e 365 kwH per day
// 		maintenance = 0.1765; //cost in euros per kWh
// 		CO2Emission = 0.0065; //6.5g per kWh
// 		buildingTime = 450; //Time it takes to build an appartment building is about 15 months 
// 		satisfaction = 3;
// 		windowNumber = 30;
// 		age = 0;
// 		break;
// 	}

// 	case 2:  {
// 		/*solar_panel() = true;
// 		double_glazing() = true;
// 		rooftop_wind_turbines() = true;*/

// 		//We consider a 500 m^2 building 
// 		srand((int)time(0));
// 		//5 appartments pay between 300-500 € per month
// 		coOwnershipBudget = (rand() % (83-50) + 50);

// 		cost = 1000000; //counting price of lot + cost of workforce + cost of all materials used
// 		energyUse = 292; // 213 kWh/m^2 per year which gives 106500 kWh per year i.e 292 kwH per day
// 		maintenance = 0.1765; //cost in euros per kWh
// 		CO2Emission = 0.0065; //6.5g per kWh
// 		buildingTime = 450; //Time it takes to build an appartment building is about 15 months 
// 		satisfaction = 7;
// 		age = 0;
// 		break;
// 	}
	
// 	}
// }

// Building::~Building() {

// }

// void Building::simulate_step(double days) {

// 	std::cout << "DEBUG: BUILDING SIMULATION CALLED" << std::endl;

// 	this->Housing::simulate_step(days);

// 	/*

// 	if (solar_panel() == true) {
// 		solarPanelAge += 1;
// 	}
			
// 	//assuming this is a button to change all windows at once i.e. you cannot call this function more than once on this house	
// 	if (double_glazing() == true) {
// 		doubleGlazingAge += 1;
		
// 	}

// 	if (rooftop_wind_turbines() == true) {
// 		rooftopWindTurbineAge += 1;
// 	}
	
	
// 	if (solarPanelAge >= solarLifetime) {
// 		//Then the solar panels are removed and we have 
// 		solar_panel() = false;
// 		solarPanelAge = 0;
// 	}

// 	if (doubleGlazingAge >= 18250) {
// 		//remove windows, they need to be changed 
// 		double_glazing() = false;
// 		doubleGlazingAge = 0;
// 	}
	
// 	if (rooftopWindTurbineAge >= windLifetime) {
// 		rooftop_wind_turbines() = false;
// 		rooftopWindTurbineAge = 0;
// 	} 
	
// 	*/

// 	std::cout << "DEBUG BUILDING 0" << std::endl;
    
// 	switch (buildingType){
// 	case 1: { //Low level house
		
// 		std::cout << "DEBUG BUILDING 1" << std::endl;
// 		if (double_glazing() == true) {
// 			energyUse *= 0.75; //when having better insulation of windows, you don't have the 25% loss of heat anymore
// 		}

// 		if (solar_panel() == true) {
// 			energyUse -= solarEnergy;
// 		}

// 		if (rooftop_wind_turbines() == true) {
// 			energyUse -= windEnergy;
// 		}

// 		maintenance += 0.1765 * energyUse * days;
// 		CO2Emission += 0.0065 * energyUse * days;  

// 		break;
// 	}

// 	case 2:  {
		
// 		std::cout << "DEBUG BUILDING 2" << std::endl;
// 		maintenance += 0.1765 * energyUse * days;
// 		CO2Emission += 0.0065 * energyUse * days;  
// 		break;
// 	}
	
// 	}
	
// }

// bool Building::solar_panel() {
// 	/*
// 	if (rooftop_wind_turbines() == true) {
// 		//we cannot have solar panels and wind turbines on the same house
// 		//a message should appear, saying that the solar_panels cannot be installed 
// 		return false;
// 		//return PanelsOff
// 	}
// 	*/

// 	//else {
// 		if (coOwnershipBudget > 25) { //the people that live in the house have enough money to install solar panels so they can 
// 			solarPanelAge = 0; 
// 			if (satisfaction < 10) {
// 				satisfaction += 1;
// 			}

// 			return PanelsOn;
// 		}

// 		else {
// 			/*
// 			if (solar_panel_subsidies() == true) {
// 				//then the household gets financial help and can install the solar_panels
// 				solarPanelAge = 0; 
// 				if (satisfaction < 10) {
// 					satisfaction += 1;
// 				}

// 				return PanelsOn;
// 			}

// 			else {
// 				//return PanelsOff;
// 				return false;
// 			}
// 			*/
// 		}
// 	//}

	
// }

// bool Building::double_glazing() {
// 	if (coOwnershipBudget > 20) {
// 		doubleGlazingAge = 0;
// 		return true;	
// 	}

// 	else {
// 		if (double_glazing_subsidies() == true) {
// 				//then the household gets financial help
// 				doubleGlazingAge = 0; 
// 				if (satisfaction < 10) {
// 					satisfaction += 1;
// 				}

// 				return true;
// 			}

// 			else {
// 				return false;
// 			}
// 	}

// }


// bool Building::rooftop_wind_turbines() {
// 	if (solar_panel() == true) {
// 		//we cannot have solar panels and wind turbines on the same house
// 		//a message should appear, saying that the turbines cannot be installed 
// 		return false;
// 		//return PanelsOff
// 	}

// 	else {
// 		if (coOwnershipBudget > 50) { //the people that live in the house have enough money to install wind turbines so they can 
// 			rooftopWindTurbineAge = 0; 
// 			if (satisfaction < 10) {
// 				satisfaction += 1;
// 			}

// 			return true;
// 		}

// 		else {
// 			if (wind_turbine_subsidies() == true) {
// 				//then the household gets financial help and can install the wind turbines
// 				rooftopWindTurbineAge = 0; 
// 				if (satisfaction < 10) {
// 					satisfaction += 1;
// 				}

// 				return true;
// 			}

// 			else {
// 				//return PanelsOff;
// 				return false;
// 			}
// 		}
// 	}
// }

// //INFORMATION DISPLAY 

// template<typename T> String to_godot_string(T s)
// {
// 	std::string standardString = std::to_string(s);
// 	godot::String godotString = godot::String(standardString.c_str());
// 	return godotString;
// }

// String Housing::get_object_info()
// {
// 	String info = this->Structure::get_object_info();

// 	info += "Age of the building in days: " + to_godot_string((double)(this->get("age"))) + String("\n");
// 	info += "CO2 Emissions: " + to_godot_string((double)(this->get("CO2Emission"))) + String("\n");
// 	info += "Energy used by the building in kWh: " + to_godot_string(this->energyUse) + String("\n");
// 	info += "Satisfaction meter, out of 10: " + to_godot_string((int)this->get("satisfaction")) + String("\n");
// 	return info;
// }

