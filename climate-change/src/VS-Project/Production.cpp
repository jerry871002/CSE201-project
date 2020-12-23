#include "Production.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>
#include <random>
using namespace godot;

String godot::Production::class_name()
{
	return "Production";
}

Production::Production()
{
}

Production::~Production()
{
}


/// <summary>
/// AGRICULTURAL PRODUCTION
/// </summary>

String godot::AgriculturalProduction::class_name()
{
	return "AgriculturalProduction";
}

AgriculturalProduction::AgriculturalProduction() {
	int type = rand()%3;
	agriculture_type(type);
	employment = 50;
	
	
}

AgriculturalProduction::~AgriculturalProduction() {}

void AgriculturalProduction::simulate_step(double days)
{
	switch(agricultureType){
		case(0):{ // wheat

			// random device class instance, source of 'true' randomness for initializing random seed
    		std::random_device rd;
    		std::mt19937 gen(rd());
			std::normal_distribution <double> wheatfieldsize(1.74, 0.04);
			requiredLand = wheatfieldsize(gen); // size of 1 wheat field in km^2
			

		break;
		}

		switch(agricultureType){ 
			//requiredLand =;
			CO2Emission = 27144; // co2 output per day for meat production in city
			waterConsumption = 31135;
			production = 1415;
			energyUse = 0;
			environmentalCost = 0;
		}
}
}
AgriculturalProduction::AgriculturalProduction(int type){
	agriculture_type(type);
}
void AgriculturalProduction::agriculture_type(int type){
	agricultureType= type; // 0 - wheat, 1 - meat, 2 - vegetables
	switch(agricultureType){
		case(0):{ // wheat

		break;
		}

		case 1: { // meat
			
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

/// <summary>
/// GOODS FACTORIES
/// </summary>

String godot::GoodsFactories::class_name()
{
	return "GoodsFactories";
}

GoodsFactories::GoodsFactories() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution <double> employees(8000, 200);
	employment = employees(gen); // number of employees of the whole city in the manufacturing/industry sector 

	energyUse = 2E6; //amount of kWh needed thorughout the sector per day

	CO2Emission = 850000; //kg of CO2 emitted per day across the manufacturing/industry sector per day
	mercuryEmission = 0.0046; //kg of mercury per day 
	arsenicEmission = 0.0048; //kg of arsenic per day
	cadmiumEmission = 0.0028; //kg of cadmium per day
	nickelEmission = 0.037; //kg of nickel per day
	leadEmission = 0.074; //kg of lead per day
	SO2Emission = 800; //kg of sulfure dioxide emitted per day
	NH3Emission = 1000; // kg of NH3 emitted per day
	NOxEmission = 1000; //kg of nitrogen oxides emitted per day
	VOCsEmission = 1500; // kg of volatile organic compounds emitted
	PMEmission = 10000; //kg of particulate matter emitted per day
}

GoodsFactories::~GoodsFactories() {}

void GoodsFactories::simulate_step(double days)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::normal_distribution <double> energy(2E6, 100000);
	energyUse += energy(gen); 

	std::normal_distribution <double> co2(850000, 10000);
	CO2Emission += co2(gen); 
	std::normal_distribution <double> mercury(0.0046, 0.0002);
	mercuryEmission += mercury(gen); 
	std::normal_distribution <double> arsenic(0.0048, 0.0002);
	arsenicEmission += arsenic(gen); 
	std::normal_distribution <double> cadmium(0.0028, 0.0001);
	cadmiumEmission += cadmium(gen); 
	std::normal_distribution <double> nickel(0.037, 0.001);
	nickelEmission += nickel(gen); 
	std::normal_distribution <double> lead(0.074, 0.001);
	leadEmission += lead(gen); 
	std::normal_distribution <double> so2(800, 20);
	SO2Emission += so2(gen); 
	std::normal_distribution <double> nh3(1000, 50);
	NH3Emission += nh3(gen); 
	std::normal_distribution <double> nox(1000, 50);
	NOxEmission += nox(gen); 
	std::normal_distribution <double> vocs(1500, 100);
	VOCsEmission += vocs(gen);
	std::normal_distribution <double> pm(10000, 300);
	PMEmission += pm(gen);
}

/// <summary>
/// SERVICES
/// </summary>

String godot::Services::class_name()
{
	return "Services";
}

Services::Services() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution <double> employees(30000, 1000);
	employment = employees(gen); // number of employees of the whole city in the service sector 

	energyUse = 250000; //amount of kWh needed thorughout the sector per day

	PMEmission = 1010; //kg of particulate matter emitted per day
	arsenicEmission = 0.002; //kg of arsenic per day
	nickelEmission = 0.012; //kg of nickel per day
	leadEmission = 0.021; //kg of lead per day
}

Services::~Services() {}

//For the moment this class stays fixed and is olny necessary for realistic data and especially employment
//We are not going to implement policies on it as it is also basically impossible in real life

void Services::simulate_step(double days)
{
	std::random_device rd;
	std::mt19937 gen(rd());


	std::normal_distribution <double> energy(250000, 5000);
	energyUse += energy(gen); 
	std::normal_distribution <double> pm(1010, 70);
	PMEmission += pm(gen);
	std::normal_distribution <double> arsenic(0.002, 0.0001);
	arsenicEmission += arsenic(gen);
	std::normal_distribution <double> nickel(0.012, 0.001);
	nickelEmission += nickel(gen);
	std::normal_distribution <double> lead(0.021, 0.002);
	leadEmission += lead(gen); 
}

/// <summary>
/// RECYCLING FACTORIES
/// </summary>

String godot::RecyclingFactories::class_name()
{
	return "RecyclingFactories";
}

RecyclingFactories::RecyclingFactories() {
}

RecyclingFactories::~RecyclingFactories() {}

void RecyclingFactories::simulate_step(double days)
{
}
