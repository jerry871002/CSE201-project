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
	employment = 8000; // number of employees of the whole city in the manufacturing/industry sector 
	CO2Emission = 850000; //kg of CO2 emitted per day across the manufacturing/industry sector 
}

GoodsFactories::~GoodsFactories() {}

void GoodsFactories::simulate_step(double days)
{
}

/// <summary>
/// SERVICES
/// </summary>

String godot::Services::class_name()
{
	return "Services";
}

Services::Services() {
	employment = 30000; // number of employees of the whole city in the service sector 
}

Services::~Services() {}

void Services::simulate_step(double days)
{
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
