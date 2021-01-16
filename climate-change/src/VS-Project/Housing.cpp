#include "Housing.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>
#include <cstdlib>
#include <ctime>
#include <Viewport.hpp>
#include <SceneTree.hpp>
#include <string>
#include <random>
#include "City.h"


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
	info += "This building produces " + to_godot_string((int)(this->get("CO2Emission"))) + " metric tonnes of CO2 yearly." + String("\n");
	info += "Yearly, the energy used by the building is " + to_godot_string((int)(this->get("energyUse"))) + " kWh." + String("\n");
	info +=  to_godot_string((int)(this->get("numberOfInhabitants"))) + " people live in this house, which has a satisfaction of "+to_godot_string((int)(this->get("satisfaction")))+ String("\n");
	
	if (get_object_type() == String("House")) {
		if ((int)(this->get("houseType"))==1){ info += "This is an old house, it probably is not as climate efficient." + String("\n");
		}
		else{ info += "This is a modern house, it likely is constrcuted with energy efficieny in mind." + String("\n"); }
	}
	if (this->PanelsOn) {
		info += "This building has solar panels ! " + String("\n") + "The panels have " + to_godot_string((int)(this->solarPanelAge)) + " days left until they are rendered obsolete." + String("\n");
	}
	else {
		info += "This building has no solar panels. Quite sad. This household would receive " + to_godot_string((int)(this->get("solar_panel_subsidies_housing"))) + " in currency if they installed solar panels." + String("\n");
	}
	if (this->rooftopWindTurbineOn) { info += "This building has a rooftop mounted wind turbine. Fancy." + String("\n"); }
	else {
		info += "If the household decides to buy wind turbines, they'd get a government subsidy of " + to_godot_string((int)(this->get("wind_turbine_subsidies"))) + String("\n");
	}
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
	register_method((char*)"_process", &Housing::_process);
	register_property<Housing, int>("housingIncome", &Housing::housingIncome, 0);
	
}


void Housing::_ready()
{
	this->Structure::_ready();
	this->get_node("MeshComponents/SolarPanels")->set("visible", false);
	this->get_node("MeshComponents/WindTurbine")->set("visible", false);
}

void Housing::_process(float delta) {
	this->Structure::_process(delta);
	rot -= delta * turnSpeed * int(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("time_speed"));
	((Mesh*)this->get_node("MeshComponents/WindTurbine"))->set("rotation_degrees", Vector3(0, - double((180 / 3.1415926535) * rot), 0));
}

void Housing::simulate_step(double days) {
	this->Structure::simulate_step(days); 
	this->Housing::panel_added_probability();	
	this->Housing::double_glazing_added_probability();
	this->Housing::roof_wind_turbines_added_probability();

	if (int(this->solarPanelAge) == 0) {

        
        double temp1 = double(1.0 - this->panel_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 
        if (r > temp3)
        {
            PanelsOn = true;
            solarPanelAge = this->solarLifetime;
            this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
			this->get_tree()->get_root()->get_node("Main/3Dworld")->set("budget", (double(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("budget")) - (double)this->get("solar_panel_subsidies_housing"))); // line that takes off from budget the subsidy
        }
        else {}
    }
    else if (int(this->solarPanelAge) > days)
    {
        this->solarPanelAge -= int(days);
    }
    else {
        this->solarPanelAge = 0;
        this->PanelsOn = false;
        this->get_node("MeshComponents/SolarPanels")->set("visible", PanelsOn);
    }

	if (int(this->doubleGlazingAge) == 0) {

        //
        double temp1 = double(1.0 - this->double_glazing_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 

        if (r > temp3)
        {
            doubleGlazingOn = true;
			this->get_tree()->get_root()->get_node("Main/3Dworld")->set("budget", (double(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("budget")) - (double)this->get("double_glazing_subsidies"))); // line that takes off from budget the subsidy
            doubleGlazingAge = 100;
        }
        else {}
    }

    else if (int(this->doubleGlazingAge) > days) {
        this->doubleGlazingAge -= int(days);
    }

    else {
        this->doubleGlazingAge = 0;
        doubleGlazingOn = false; 
    }

	if (int(this->rooftopWindTurbineAge) == 0) {

        //
        double temp1 = double(1.0 - this->roof_wind_turbines_probability);
        double temp2 = double(days / 365.0);
        double temp3 = pow(temp1, temp2);

        double r = double(rand()) / double((RAND_MAX + 1.)); // gives  double between 0 and 1 
        if (r > temp3)
        {
            rooftopWindTurbineOn = true;
            rooftopWindTurbineAge = 100;
            this->get_node("MeshComponents/WindTurbine")->set("visible", rooftopWindTurbineOn);
			this->get_tree()->get_root()->get_node("Main/3Dworld")->set("budget", (double(this->get_tree()->get_root()->get_node("Main/3Dworld")->get("budget")) - (double)this->get("wind_turbine_subsidies"))); // line that takes off from budget the subsidy

        }
        else {}
    }
    else if (int(this->rooftopWindTurbineAge) > days) {
        this->rooftopWindTurbineAge -= int(days);
    }

    else {
        this->rooftopWindTurbineAge = 0;
        rooftopWindTurbineOn = false;
        this->get_node("MeshComponents/WindTurbine")->set("visible", rooftopWindTurbineOn);

    } 

	
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
		//The solar panels are removed after a certain amount of time (the average lifetime of a panel)
		this->PanelsOn = false;
		this->solarPanelAge = 0;
	}

	if (this->doubleGlazingAge >= 365) {
		this->doubleGlazingOn = false;
		this->doubleGlazingAge = 0;
	} 

	if (this->rooftopWindTurbineAge >= windLifetime) {
		this->rooftopWindTurbineOn = false;
		this->rooftopWindTurbineAge = 0;
	} 
};

int Housing::get_inhabitants() {
	return this->numberOfInhabitants;
}

void Housing::set_inhabitants(int value) {
	this->numberOfInhabitants = value;
}

double Housing::get_min_income() {
    return this->minIncome;
}

double Housing::get_max_income() {
	return this->maxIncome;
}

double Housing::get_solar_panel_age() {
    return this->solarPanelAge;
}

double Housing::get_double_glazing_age() {
    return this->doubleGlazingAge;
}

double Housing::get_satisfaction() {
	double panelsF = 0;
	double roofturbineF = 0;
	double treesF = 0;
	if (this->PanelsOn) { panelsF= 2; };
    if (this->rooftopWindTurbineOn) { roofturbineF = 1.1; };
	if (this->get_main_type() == "Housing" || (this->get_main_type() == "Shop") && this->get_object_type() == "Mall") {
		if (this->get_node("MeshComponents/Trees")->get("visible")) {
			treesF = 3;
		}
	}
	if (this->satisfaction + panelsF + roofturbineF + treesF > 10){
		return 10;
	}
	else {
		return this->satisfaction + panelsF + roofturbineF + treesF;
	}

}


void Housing::panel_added_probability(){
	double panelCost;
    double panel_subsidies = this->get("solar_panel_subsidies_housing"); // input from user of how much are the subsidies
    // double income_indexed = 0.5;
    panelCost = this->solarCost - panel_subsidies;  
    if (panelCost < 0) {
		panelCost = 0; //cannot have a negative solar panel cost
	}

	//convert housingIncome to a value between 0 and 1 to be used in the probbility
	double housingIncomeIndexed = 1 - ((this->maxIncome - this->housingIncome) / this->maxIncome);

	panel_probability = (((this->solarCost - panelCost)/this->solarCost)*50 + (this->solarSatisfaction/10)*15 + housingIncomeIndexed*35)/100;

	if (PanelsOn == true) {
		panel_probability = 0;
	}

	//We do not want to put wind turbines if there are already solar panels
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
	double panelsF = 1;
	
	if (this->PanelsOn) { panelsF = 0.7; };

	double trees = 0;
	
	if (this->get_node("MeshComponents/Trees")->get("visible")) {
		trees = 0.2;  // 10 trees absorb 200 kilos of co2 a year
	}
	
	
	return (double)(((this->CO2Emission)-trees)  * panelsF);
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
	
    return ((double)(this->energyUse))*panelsF*turbineF*glazingF;
}

double House::get_environmentalcost() {
    return this->environmentalCost;
}


House::House() 
{

}

void House::_ready()
{
	this->Housing::_ready();
	int type = 1;
	this->houseType = type;

	double employees = (double)((City*)((this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld"))))->get("numberOfEmployees");
	int population = (int)((City*)((this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld"))))->get("population");
	double unemployment = (double)(100 - 100 * fmin((double)1, (double)(employees / (fmax(population, 1)))) + 0.5);

	if (type == 1) {
		//Low level house
		//definition of low level house, has no solar panels, no double glazing and no wind turbines on roof 
		this->PanelsOn = false;
		this->doubleGlazingOn = false;
		this->rooftopWindTurbineOn = false;


		srand((int)time(0));
		//minimum wage 53 € per day - get money even on saturday and saturday 
		// i take max to be 333€

		this->numberOfInhabitants = (rand() % (6) + 1);


		if (this->numberOfInhabitants >= 2) {
			//have two salaries 
			//housingIncome = (rand() % (maxIncome - minIncome)) + minIncome + (rand() % (maxIncome - minIncome)) + minIncome;
			srand((int)time(0));
			double probability = (rand() % (100));
			if (probability <= unemployment) {
				this->housingIncome = 23 * 2;
			}
			else {
				this->housingIncome = fmax(minIncome, normalGenerator(50, 70)) + fmax(minIncome, normalGenerator(50, 70));
			}
		}

		else {
			//housingIncome = (rand() % (maxIncome - minIncome)) + minIncome;
			srand((int)time(0));
			double probability = (rand() % (100));
			if (probability <= unemployment) {
				housingIncome = 23;
			}
			else {
				housingIncome = fmax(minIncome, normalGenerator(50, 70));
			}
		}

		//Attributes for a low level house 
		this->cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
		this->energyUse = 25000; //25000kWh per year i.e. 13.69 kWh per day (from heating and all )
		this->maintenance = 0.1765; //cost in euros per kWh
		this->CO2Emission = 3.51; ////tons per year 
		this->buildingTime = 140; //in average, building a house takes about 140 days
		this->satisfaction = 3;
		//satisfaction = 3; //assuming we are on a scale from 0 to 10

		//attributes special to this class
		windowNumber = 5;
		srand((int)time(0));
		this->age = (rand() % (20 * 365) + 1);


	}
	else {
		// High Level House 
		//has properties : double glazing 

		this->PanelsOn = false;
		this->doubleGlazingOn = true;
		this->rooftopWindTurbineOn = false;

		srand((int)time(0));
		this->numberOfInhabitants = (rand() % (6) + 1);
		if (this->numberOfInhabitants >= 2) {
			srand((int)time(0));
			double probability = (rand() % (100));
			if (probability <= unemployment) {
				housingIncome = 23 * 2;
			}
			else {
				housingIncome = fmax(minIncome, normalGenerator(50, 70)) + fmax(minIncome, normalGenerator(50, 70));
			}
		}

		else {
			srand((int)time(0));
			double probability = (rand() % (100));
			if (probability <= unemployment) {
				housingIncome = 23;
			}
			else {
				housingIncome = fmax(minIncome, normalGenerator(50, 70));
			}
		}

		//attributes from structure class
		cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
		energyUse = 20000; //20000kWh per year i.e. 54.79 kWh per day (from heating and all )
		maintenance = 0.1765; //cost in euros per kWh
		this->CO2Emission = 3.51; //tons per year
		buildingTime = 140; //in average, building a house takes about 140 days
		this->satisfaction = 7;
		//satisfaction = 10; //assuming we are on a scale from 0 to 10
		srand((int)time(0));
		this->age = (rand() % (20 * 365) + 1);
	}


}


// This function is necessary to properly initialize the houses in Godot and to make sure they have the correct values


void House::_register_methods()
{
	register_method((char*)"_ready", &House::_ready);
	register_property<House, int>("houseType", &House::set_houseType, &House::get_houseType, 1);
}


double Housing::normalGenerator(double mean, double stdDev){
    static std::default_random_engine generator(time(0));
    std::normal_distribution<double> distribution(mean,stdDev);
    return distribution(generator); 
}

void House::set_houseType(int type)
{
	this->houseType = type;

	double unemployment = 0;

	if (type == 1) {
	//Low level house
	//definition of low level house, has no solar panels, no double glazing and no wind turbines on roof 
		PanelsOn = false;
		doubleGlazingOn = false;
		rooftopWindTurbineOn = false;

		
		srand((int)time(0));
		//minimum wage 53 € per day - get money even on saturday and saturday 
		// i take max to be 333€

		this->numberOfInhabitants = (rand() % (6) + 1);
		

		if (this->numberOfInhabitants >= 2) {
			//have two salaries 
			//housingIncome = (rand() % (maxIncome - minIncome)) + minIncome + (rand() % (maxIncome - minIncome)) + minIncome;
			srand((int)time(0));
			double probability = (rand() % (100));
			if(probability <= unemployment){
				housingIncome = 23*2;
			}
			else {
				housingIncome = fmax(minIncome, normalGenerator(50, 70)) + fmax(minIncome, normalGenerator(50, 70));
			}
		}

		else {
			//housingIncome = (rand() % (maxIncome - minIncome)) + minIncome;
			srand((int)time(0));
			double probability = (rand() % (100));
			if (probability <= unemployment) {
				housingIncome = 23;
			}
			else {
				housingIncome = fmax(minIncome, normalGenerator(50, 70));
			}
		}

		//Attributes for a low level house 
		cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
		energyUse = 25000; //25000kWh per year i.e. 13.69 kWh per day (from heating and all )
		maintenance = 0.1765; //cost in euros per kWh
		this->CO2Emission = 3.51; ////tons per year 
		buildingTime = 140; //in average, building a house takes about 140 days
		this->satisfaction = 3;
		//satisfaction = 3; //assuming we are on a scale from 0 to 10

		//attributes special to this class
		windowNumber = 5;
		srand((int)time(0));
		this->age = (rand() % (20*365) + 1);
		

	}
	else {
		// High Level House 
		//has properties : double glazing 

		PanelsOn = false;
		doubleGlazingOn = true;
		rooftopWindTurbineOn = false;

		srand((int)time(0));
		this->numberOfInhabitants = (rand() % (6) + 1);
		if (this->numberOfInhabitants >= 2) {
			srand((int)time(0));
			double probability = (rand() % (100));
			if (probability <= unemployment) {
				housingIncome = 23 * 2;
			}
			else {
				housingIncome = fmax(minIncome, normalGenerator(50, 70)) + fmax(minIncome, normalGenerator(50, 70));
			}
		}

		else {
			srand((int)time(0));
			double probability = (rand() % (100));
			if (probability <= unemployment) {
				housingIncome = 23;
			}
			else {
				housingIncome = fmax(minIncome, normalGenerator(50, 70));
			}
		}

		//attributes from structure class
		cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
		energyUse = 20000; //20000kWh per year i.e. 54.79 kWh per day (from heating and all )
		maintenance = 0.1765; //cost in euros per kWh
		this->CO2Emission = 3.51; //tons per year
		buildingTime = 140; //in average, building a house takes about 140 days
		this->satisfaction = 8;
		//satisfaction = 10; //assuming we are on a scale from 0 to 10
		srand((int)time(0));
		this->age = (rand() % (20 * 365) + 1);
	}

}

int House::get_houseType()
{
	return this->houseType;
}

void House::house_type() 
{

	int type = this->get("houseType");

	
}

House::~House() {

}



void House::simulate_step(double days) {


	this->Housing::simulate_step(days);
}

/// <summary>
///	BUILDING CLASS
/// </summary>
double Building::get_co2emissions() {
	double panelsF = 1;
	if (this->PanelsOn) { panelsF = 0.7; };

	double trees = 0;
	
	if (this->get_node("MeshComponents/Trees")->get("visible")) {
		trees = 0.2;  // 10 trees absorb 200 kilos of co2 a year
	}

	return (double)(((this->CO2Emission)-trees) * panelsF);
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


void Building::_register_methods()
{
	register_method((char*)"_ready", &Building::_ready);
	register_method((char*)"_process", &Building::_process);
}

Building::Building() {
	
}

Building::~Building() {

}

void Building::_ready() {
	this->Housing::_ready();
	PanelsOn = false;
	rooftopWindTurbineOn = false;
	srand((int)time(0));
	this->age = (rand() % (20 * 365) + 1);

	windowNumber = 30;
	cost = 1000000; //counting price of lot + cost of workforce + cost of all materials used
	maintenance = 0.1765; //cost in euros per kWh

	srand((int)time(0));

	housingIncome = 0;
	double incomeEach = 0;

	//double employees = (double)((City*)((this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld"))))->get("numberOfEmployees");
	//int population = (int)((City*)((this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld"))))->get("population");
	double employees = 80;
	int population = 100;
	double unemployment = (double)(100 - 100 * fmin((double)1, (double)(employees / (fmax(population, 1)))) + 0.5);

	this->numberOfInhabitants = (rand() % (10) + 50);
	this->buildingType = (rand() % 2 + 1);

	for (int i = 0; i < numberOfInhabitants / 2; i++) { //I took half inhabitants are children
		srand((int)time(0));
		double probability = (rand() % (100));
		if (probability <= unemployment) {
			incomeEach += 23;
		}
		else {
			incomeEach += fmax(minIncome, normalGenerator(50, 70));
		}
	}

	//This is to compute an average wage in the building so that the probability functions to add solar panels etc still work the same as for a house
	housingIncome = incomeEach / this->numberOfInhabitants;


	if (buildingType == 1) { //Low level building
		doubleGlazingOn = false;

		//Initializing the values:
		energyUse = 133125; // 266.25 kWh/m^2 per year which gives 133125 kWh per year i.e 365 kwH per day
		CO2Emission = 17.55; //tons per year from gas heating 
		buildingTime = 450; //Time it takes to build an appartment building is about 15 months 
		this->satisfaction = 3;

	}

	else { //High level building
		doubleGlazingOn = true;

		//Initializing the values:
		energyUse = 106500; // 213 kWh/m^2 per year which gives 106500 kWh per year i.e 292 kwH per day	
		CO2Emission = 17.55;
		buildingTime = 450; //Time it takes to build an appartment building is about 15 months 
		this->satisfaction = 7;
	}


}

void Building::simulate_step(double days) {
	this->Housing::simulate_step(days);
}
	


