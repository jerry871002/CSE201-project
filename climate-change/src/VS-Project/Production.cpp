#include "Production.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>
#include <random>
#include <string>
using namespace godot;

String godot::Production::class_name() {
	return "Production";
}

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

/// <summary>
/// AGRICULTURAL PRODUCTION
/// </summary>

String godot::AgriculturalProduction::class_name() {
	return "AgriculturalProduction";
}

AgriculturalProduction::AgriculturalProduction() {
	int type = rand()%3;
	agriculture_type(type);

	subsidy = false;
	}


AgriculturalProduction::~AgriculturalProduction() {

}

void AgriculturalProduction::simulate_step(double days) {
	age+=days;
	//prohibite_pesticide();
	switch(agricultureType){
		case(0):{ // wheat
		production=requiredLand*fertility*days; //output over the time period
		waterConsumption=requiredLand*fertility*days*1500;
		CO2Emission=0.59*requiredLand*fertility*days;
		maintenance = 144*days*requiredLand;
		if (pesticideProhibited==0){
			CO2Emission=9400*requiredLand*days;
			maintenance+=25*requiredLand;
			}
		if (fertilizerProhibited == 0){
			maintenance+=45*requiredLand;
		}
		break;
		}

		case(1):{ 
			std::random_device rd;
    		std::mt19937 gen(rd());
			std::normal_distribution <double> foodformeatfieldsize(1.2, 0.04);
			requiredLand = foodformeatfieldsize(gen);
			CO2Emission += 27144;
			waterConsumption += 31135; 
			production += 1415; 
			break;
		}
	}
}


AgriculturalProduction::AgriculturalProduction(int type){
	agriculture_type(type);
}
void AgriculturalProduction::agriculture_type(int type){
	agricultureType = type; // 0 - wheat, 1 - meat, 2 - vegetables
	employment = 50;
	//prohibite_pesticide();
	switch(agricultureType){
		case(0):{ // wheat
		    std::random_device rd;
    		std::mt19937 gen(rd());
			std::normal_distribution <double> wheatfieldsize(1.74, 0.04);
			requiredLand = wheatfieldsize(gen); // size of 1 wheat field in km^2
			std::normal_distribution <double> wheatferltility(464000, 40000);
			fertility = wheatferltility(gen); //production of wheat per km^2
			std::normal_distribution <double> wheatfieldcost(595000, 1000);
			cost = wheatfieldcost(gen)*requiredLand; //of land in euros
			production = fertility *requiredLand; //output of wheat in kg per day
			waterConsumption = 1500*production; //water litres per day
			CO2Emission =0.59*production; //co2 kg per day
			pesticideProhibited = false;
			fertilizerProhibited = false;
			GMOProhibited = false;
			std::normal_distribution <double> wheatsatisfaction(5.15, 0.15);
			satisfaction = wheatsatisfaction(gen);
		break;
		}
		case 1: { // meat
		CO2Emission = 27144; // co2 output per day for meat production in city
		waterConsumption = 31135; //liters per day
		production = 1415; //in kg of meat every day
		energyUse = 0;
		environmentalCost = 0;   
			break;
		}
	}
}
double AgriculturalProduction::get_satisfaction(){
	return this->satisfaction;
}
double AgriculturalProduction::get_co2emissions(){
	return this->CO2Emission;
}		
double AgriculturalProduction::get_energyuse(){
	return this->energyUse;
}
double AgriculturalProduction::get_environmentalcost(){
	return 0;
}

void AgriculturalProduction::prohibite_pesticide(){
	double pesticidePolicy = this->get("Pesticides"); // from user
	switch (agricultureType)
	{
	case (0):
		if ((pesticideProhibited==1)&&(pesticidePolicy==0)){
			fertility*=1.15;
			satisfaction*=0.85;
		}
		else if ((pesticideProhibited==0)&&(pesticidePolicy==1)){
			fertility/=1.15;
			satisfaction/=0.85;
		}
		break;
	}
	pesticideProhibited = pesticidePolicy;
}

/// <summary>
/// GOODS FACTORIES
/// </summary>

String godot::GoodsFactories::class_name()
{
	return "GoodsFactories";
}

GoodsFactories::GoodsFactories() {
	age = 0;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution <double> employees(100, 60);
	employment = employees(gen); // number of employees in the factory 
	factoryGDP = employment * 90; //in euros per year

	energyUse = 100*employment; //amount of kWh needed for one factory per day

	CO2Emission = 42.5*employment; //kg of CO2 emitted per day 
	mercuryEmission = 2.3E-7*employment; //kg of mercury per day 
	arsenicEmission = 2.4E-7*employment; //kg of arsenic per day
	cadmiumEmission = 1.E-7*employment; //kg of cadmium per day
	nickelEmission = 1.85E-6*employment; //kg of nickel per day
	leadEmission = 3.7E-6*employment; //kg of lead per day
	SO2Emission = 0.04*employment; //kg of sulfure dioxide emitted per day
	NH3Emission = 0.05*employment; // kg of NH3 emitted per day
	NOxEmission = 0.05*employment; //kg of nitrogen oxides emitted per day
	VOCsEmission = 0.075*employment; // kg of volatile organic compounds emitted
	PMEmission = 0.5*employment; //kg of particulate matter emitted per day

	subsidy = false;
}

GoodsFactories::~GoodsFactories() {}

void GoodsFactories::simulate_step(double days)
{
	age += days;

	std::random_device rd;
	std::mt19937 gen(rd());

	double green = 1; //needed for the green_subsidy policy

	if (subsidy_green > 0) {
		int value = 10;
		subsidy = true;
		if (subsidy_green <= 30000) {
			value = 10;
		}
		if (60000 >= subsidy_green >= 30000) {
			value = 7;
		}
		if (maximum_CO2 > 60000) {
			value = 5;
		}
		srand((int)time(0));
		double chance = (rand() % (value));
		if (chance < 6) {
			std::normal_distribution <double> employees(150, 60);
			employment = employees(gen); // number of employees in the factory 
			green = 1 - (employment * 0.02);
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
		srand((int)time(0));
		double chance = (rand() % (maxi));
		if (employment<=60 && chance < 1) {
			employment = 0;
		}
		if (60 < employment <= 120 && chance < 2) {
			employment = 0;
		}
		if (employment > 120 && chance < 5) {
			employment = 0;
		}
		std::normal_distribution <double> co2(maximum_CO2, 0.5);
		CO2Emission = (co2(gen) * employment) * green;
	}
	else {
		std::normal_distribution <double> co2(42.5, 0.5);
		CO2Emission = (co2(gen) * employment) * green;
	}

	std::normal_distribution <double> mercury(2.3E-7, 1E-8);
	mercuryEmission = (mercury(gen)* employment)*green;
	std::normal_distribution <double> arsenic(2.4E-7, 1E-8);
	arsenicEmission = (arsenic(gen)* employment)* green;
	std::normal_distribution <double> cadmium(1.E-7, 5E-9);
	cadmiumEmission = (cadmium(gen)* employment) * green;
	std::normal_distribution <double> nickel(1.85E-6, 5E-8);
	nickelEmission = (nickel(gen)* employment) * green;
	std::normal_distribution <double> lead(3.7E-6, 5E-8);
	leadEmission = (lead(gen)* employment) * green;
	std::normal_distribution <double> so2(0.04, 0.001);
	SO2Emission = (so2(gen)* employment) * green;
	std::normal_distribution <double> nh3(0.05, 0.0025);
	NH3Emission = (nh3(gen)* employment) * green;
	std::normal_distribution <double> nox(0.05, 0.0025);
	NOxEmission = (nox(gen)* employment) * green;
	std::normal_distribution <double> vocs(0.075, 0.005);
	VOCsEmission = (vocs(gen)* employment) * green;
	std::normal_distribution <double> pm(0.5, 0.15);
	PMEmission = (pm(gen)* employment) * green;

	std::normal_distribution <double> energy(100, 10);
	energyUse = energy(gen)* employment * green;
}

/// <summary>
/// SERVICES
/// </summary>

String godot::Services::class_name()
{
	return "Services";
}

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
	age += days;

	std::random_device rd;
	std::mt19937 gen(rd());


	std::normal_distribution <double> energy(250000, 5000);
	energyUse = energy(gen); 
	std::normal_distribution <double> pm(1010, 70);
	PMEmission = pm(gen);
	std::normal_distribution <double> arsenic(0.002, 0.0001);
	arsenicEmission = arsenic(gen);
	std::normal_distribution <double> nickel(0.012, 0.001);
	nickelEmission = nickel(gen);
	std::normal_distribution <double> lead(0.021, 0.002);
	leadEmission = lead(gen); 
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

	info += "Employment: " + to_godot_string(this->employment) + String("\n");
	info += "Energy used by the building in kWh: " + to_godot_string(this->energyUse) + String("\n");
	info += "CO2 Emissions: " + to_godot_string((double)(this->get("CO2Emission"))) + String("\n");
	if (subsidy == true) {
		info += "This factory receives a green subsidy" + String("\n");
	}
	info += "Satisfaction meter, out of 10: " + to_godot_string((int)this->get("satisfaction")) + String("\n");
	return info;
}