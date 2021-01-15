#include "Production.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>
#include <random>
#include <string>
#include <SceneTree.hpp>
#include <Viewport.hpp>
#include "City.h"

using namespace godot;


Production::Production() {
}

Production::~Production() {
}

double Production::get_PMEemission()
{
	return this->PMEmission;
}

double Production::get_NH3emission()
{
	return this->NH3Emission;
}

double Production::get_SO2emission()
{
	return this->SO2Emission;
}

double Production::get_NOxemission()
{
	return this->NOxEmission;
}

double Production::get_VOCsemission()
{
	return this->VOCsEmission;
}

double Production::get_mercury_emission()
{
	return this->mercuryEmission;
}

double Production::get_arsenic_emission()
{
	return this->arsenicEmission;
}

double Production::get_cadmium_emission()
{
	return this->cadmiumEmission;
}

double Production::get_lead_emission()
{
	return this->leadEmission;
}

double Production::get_nickel_emission()
{
	return this->nickelEmission;

}

void Production::simulate_step(double days) {
	this->Structure::simulate_step(days);
}

/// <summary>
/// AGRICULTURAL PRODUCTION
/// </summary>



AgriculturalProduction::AgriculturalProduction() {
	int type = rand() % 3;
	agriculture_type(type);

	subsidy = false;
}


AgriculturalProduction::~AgriculturalProduction() {

}

void AgriculturalProduction::simulate_step(double days) 
{
	
	this->Production::simulate_step(days);

	switch (agricultureType) {
	case(0): { // wheat
		if ((fertilizerBefore == 0) && (fertilizerProhibited == 1)) {
			fertility *= 0.65;
			satisfaction /= 0.95;
		}
		else if ((fertilizerBefore == 1) && (fertilizerProhibited == 0)) {
			fertility /= 0.65;
			satisfaction *= 0.95;
		}
		if ((pesticideBefore == 0) && (pesticideProhibited == 1)) {
			fertility /= 1.15;
			satisfaction /= 0.85;
		}
		else if ((pesticideBefore == 1) && (pesticideProhibited == 0)) {
			fertility *= 1.15;
			satisfaction *= 0.85;
		}
		GMOBefore = GMOProhibited;
		pesticideBefore = pesticideProhibited;
		fertilizerBefore = fertilizerProhibited;
		production = requiredLand * fertility; //output over the time period
		waterConsumption = requiredLand * production * 1000* 1500;
		CO2Emission = 0.4 * production; //co2 tonne per year
		maintenance = 144 * 365 * requiredLand; //maintenance in euros per year
		if (fertilizerProhibited == 0) {
			maintenance += 45 * requiredLand * 365;
			CO2Emission += 0.3 * CO2Emission;
		}
		if (pesticideProhibited == 0) { //depends on the land size
			CO2Emission += 9.4 * requiredLand;
			maintenance += 25 * requiredLand*365;
		}
		break;
	}

	case(1): {
		production = (requiredLand * fertility); //required land taken so that this is production in ton per year
		waterConsumption = production * 1000 * 22; //liters per kg so had to convert production back to kg
		CO2Emission = 19.18 * production;

		/*std::random_device rd;
		std::mt19937 gen(rd());
		std::normal_distribution <double> foodformeatfieldsize(1.2, 0.04);
		requiredLand = foodformeatfieldsize(gen);
		CO2Emission += 27144;
		waterConsumption += 31135;
		production += 1415;*/
		break;
	}
	}
}


AgriculturalProduction::AgriculturalProduction(int type) {
	agriculture_type(type);
}

void AgriculturalProduction::agriculture_type(int type) {
	agricultureType = type; // 0 - wheat, 1 - meat, 2 - vegetables
	employment = 50;
	switch (agricultureType) {
	case(0): { // wheat
		std::random_device rd;
		std::mt19937 gen(rd());
		std::normal_distribution <double> wheatfieldsize(1.74, 0.04);
		requiredLand = wheatfieldsize(gen); // size of 1 wheat field in km^2
		std::normal_distribution <double> wheatferltility(0.464, 0.004);
		fertility = wheatferltility(gen); //production of wheat in tonne per km^2
		std::normal_distribution <double> wheatfieldcost(595000, 1000);
		cost = wheatfieldcost(gen) * requiredLand; //of land in euros
		production = fertility * requiredLand; //output of wheat in tonne per year
		waterConsumption = 1500 * production*1000; //water litres per year
		CO2Emission = 0.52 * production; //co2 tonne per year
		pesticideProhibited = 0;
		fertilizerProhibited = 0;
		GMOProhibited = 0;
		fertilizerBefore = 0;
		GMOBefore = 0;
		pesticideBefore = 0;
		std::normal_distribution <double> wheatsatisfaction(5.15, 0.15);
		satisfaction = wheatsatisfaction(gen);
		break;
	}
	case 1: { // meat
		std::random_device rd;
		std::mt19937 gen(rd());
		std::normal_distribution <double> foodformeatfieldsize(2, 0.1); //size of field giving a yearly production for our city
		requiredLand = foodformeatfieldsize(gen); // size
		std::normal_distribution <double> cropsfertility(0.2, 0.005); //  ton per square meter
		fertility = cropsfertility(gen); 
		production = (fertility * requiredLand);
		waterConsumption = 22 * production * 1000; //22L of water per 1 kg of production
		CO2Emission = 19.18 * production; //co2 per ton of production
		energyUse = 0;
		environmentalCost = 0;   
		satisfaction = 2;
			
		/*
		CO2Emission = 27144; // co2 output per day for meat production in city
		waterConsumption = 31135; //liters per day
		production = 1415; //in kg of meat every day
		energyUse = 0;
		environmentalCost = 0;
		break;*/
	}
	}
}
double AgriculturalProduction::get_satisfaction() {
	return this->satisfaction;
}
double AgriculturalProduction::get_co2emissions() {
	return this->CO2Emission;
}
double AgriculturalProduction::get_energyuse() {
	return this->energyUse;
}
double AgriculturalProduction::get_environmentalcost() {
	return 0;
}


/// <summary>
/// GOODS FACTORIES
/// </summary>



GoodsFactories::GoodsFactories() {
	age = 0;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution <double> employees(100, 60);
	employment = employees(gen); // number of employees in the factory 
	factoryGDP = employment * 90; //in euros per year
	std::normal_distribution <double> sat(5, 3);
	satisfaction = sat(gen);

	energyUse = 100 * employment; //amount of kWh needed for one factory per day

	CO2Emission = 42.5E-3 * employment; //ton of CO2 emitted per day 
	mercuryEmission = 2.3E-10 * employment; //ton of mercury per day 
	arsenicEmission = 2.4E-10 * employment; //ton of arsenic per day
	cadmiumEmission = 1.E-10 * employment; //ton of cadmium per day
	nickelEmission = 1.85E-9 * employment; //ton of nickel per day
	leadEmission = 3.7E-9 * employment; //ton of lead per day
	SO2Emission = 0.04E-3 * employment; //ton of sulfure dioxide emitted per day
	NH3Emission = 0.05E-3 * employment; // ton of NH3 emitted per day
	NOxEmission = 0.05E-3 * employment; //ton of nitrogen oxides emitted per day
	VOCsEmission = 0.075E-3 * employment; // ton of volatile organic compounds emitted
	PMEmission = 0.5E-3 * employment; //ton of particulate matter emitted per day

	subsidy = false;
}

GoodsFactories::~GoodsFactories() {}

void godot::GoodsFactories::_process(float delta)
{
	this->Structure::_process(delta);
	this->get_node("Smoke")->get_child(0)->set("speed_scale", int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed")));
	this->get_node("Smoke")->get_child(1)->set("speed_scale", int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed")));
}

void GoodsFactories::simulate_step(double days)
{
	this->Production::simulate_step(days);

	double maximum_CO2 = this->get("maximum_CO2"); // input from user
	double subsidy_green = this->get("subsidy_green"); // input from user

	std::random_device rd;
	std::mt19937 gen(rd());

	double green = 1; //needed for the green_subsidy policy

	if (subsidy_green > 0) {
		int value = 10;
		if (subsidy_green <= 30000) {
			value = 10;
		}
		if (60000 >= subsidy_green >= 30000) {
			value = 7;
		}
		if (subsidy_green > 60000) {
			value = 5;
		}
		srand((int)time(0));
		double chance = (rand() % (value));
		if (chance < 6) {
			subsidy = true;
			std::normal_distribution <double> employees(150, 60);
			employment = employees(gen); // number of employees in the factory 
			green = 1 - (employment * 0.005);
		}
	}

	if (maximum_CO2 > 0) {
		int maxi = 10;
		if (maximum_CO2 >= 30) {
			maxi = 10;
		}
		if (30 > maximum_CO2 >= 20) {
			maxi = 8;
		}
		if (maximum_CO2 < 20) {
			maxi = 6;
		}
		if (maximum_CO2 < 10) {
			maxi = 4;
		}
		srand((int)time(0));
		double chance = (rand() % (maxi));
		if (employment <= 60 && chance < 2) {
			employment = 0;
			factory_closed = true;
			age = 0;
		}
		if (60 < employment <= 120 && chance < 4) {
			employment = 0;
			factory_closed = true;
			age = 0;
		}
		if (employment > 120 && chance < 6) {
			employment = 0;
			factory_closed = true;
			age = 0;
			this->get_node("Smoke")->set("visible", false);
		}
		else {
			this->get_node("Smoke")->set("visible", true);
		}
		std::normal_distribution <double> co2(maximum_CO2, 0.5);
		CO2Emission = (co2(gen) * employment) * green * 0.001 * 365; // ton per year
	}
	else {
		std::normal_distribution <double> co2(42.5, 0.5);
		CO2Emission = (co2(gen) * employment) * green * 0.001 * 365;
	}

	std::normal_distribution <double> mercury(2.3E-7, 1E-8);
	mercuryEmission = (mercury(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> arsenic(2.4E-7, 1E-8);
	arsenicEmission = (arsenic(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> cadmium(1.E-7, 5E-9);
	cadmiumEmission = (cadmium(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> nickel(1.85E-6, 5E-8);
	nickelEmission = (nickel(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> lead(3.7E-6, 5E-8);
	leadEmission = (lead(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> so2(0.04, 0.001);
	SO2Emission = (so2(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> nh3(0.05, 0.0025);
	NH3Emission = (nh3(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> nox(0.05, 0.0025);
	NOxEmission = (nox(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> vocs(0.075, 0.005);
	VOCsEmission = (vocs(gen) * employment) * green * 0.001 * 365;
	std::normal_distribution <double> pm(0.5, 0.15);
	PMEmission = (pm(gen) * employment) * green * 0.001 * 365;

	std::normal_distribution <double> energy(100, 10);
	energyUse = energy(gen) * employment * green * 365;
}

/// <summary>
/// SERVICES
/// </summary>


Services::Services() {
	age = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution <double> employees(30000, 1000);
	employment = employees(gen); // number of employees of the whole city in the service sector 

	energyUse = 250000; //amount of kWh needed thorughout the sector per day

	PMEmission = 1010; //kg of particulate matter emitted per day
	arsenicEmission = 0.002; //kg of arsenic per day
	nickelEmission = 0.012; //kg of nickel per day
	leadEmission = 0.021; //kg of lead per day

	subsidy = false;
}

Services::~Services() {}

//For the moment this class stays fixed and is olny necessary for realistic data and especially employment
//We are not going to implement policies on it as it is also basically impossible in real life

void Services::simulate_step(double days)
{
	this->Production::simulate_step(days);

	std::random_device rd;
	std::mt19937 gen(rd());


	std::normal_distribution <double> energy(250000, 5000);
	energyUse = energy(gen) * 365;
	std::normal_distribution <double> pm(1010, 70);
	PMEmission = pm(gen) * 0.001 * 365;
	std::normal_distribution <double> arsenic(0.002, 0.0001);
	arsenicEmission = arsenic(gen) * 0.001 * 365;
	std::normal_distribution <double> nickel(0.012, 0.001);
	nickelEmission = nickel(gen) * 0.001 * 365;
	std::normal_distribution <double> lead(0.021, 0.002);
	leadEmission = lead(gen) * 0.001 * 365;
}

// INOFRMATION DISPLAY

void Production::_register_methods()
{
}

template<typename T> String to_godot_string(T s)
{
	std::string standardString = std::to_string(s);
	godot::String godotString = godot::String(standardString.c_str());
	return godotString;
}

String Production::get_object_info()
{
	String info = this->Structure::get_object_info();
	if (subsidy == true && factory_closed == false) {
		info += "This factory receives a green subsidy which helps it grow and cause less environmental damage." + String("\n");
	}
	if (factory_closed == true) {
		info += "This factory is closed due to the taxes linked to the maximum carbon law." + String("\n");
	}
	info += "This building produces " + to_godot_string((int)(this->get("CO2Emission"))) + " metric tonnes of CO2 yearly." + String("\n");
	info += "Age: " + to_godot_string((int)(this->age)) + String("\n");
	info += "Employment: " + to_godot_string((int)(this->employment)) + String("\n");
	info += "Energy used by the building in kWh per year: " + to_godot_string((int)(this->energyUse)) + String("\n");
	info += "Satisfaction meter, out of 10: " + to_godot_string((int)this->get("satisfaction")) + String("\n");
	return info;
}