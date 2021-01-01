#include "Production.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>
#include <random>
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
	employment = 50;
	switch (agricultureType){
	case 1: { 
        CO2Emission = 27144; // co2 output per day for meat production in city
		waterConsumption = 31135; //liters per day
		production = 1415; //in kg of meat every day
		energyUse = 0;
		environmentalCost = 0;   
		break;
		}

	 }
}

AgriculturalProduction::~AgriculturalProduction() {

}

void AgriculturalProduction::simulate_step(double days) {
	switch(agricultureType){
		case(0):{ // wheat
			// random device class instance, source of 'true' randomness for initializing random seed
    		std::random_device rd;
    		std::mt19937 gen(rd());
			std::normal_distribution <double> wheatfieldsize(1.74, 0.04);
			requiredLand = wheatfieldsize(gen); // size of 1 wheat field in km^2
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
	double factories = 200;

	/*We model 200 factories on average with approximately 8000 employees overall. We approximate for the constructor 100 small factories with 0-20 
	employees each, 90 medium ones with 20-100 employees each and 10 big ones with 100-220 employees each, for the policies' implementation 
	*/

	energyUse = 2E6; //amount of kWh needed throughout the sector per day

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

	if (maximum_CO2() < 10E6) {
		double maxi = maximum_CO2();
		std::normal_distribution <double> co2(maxi, 100);
		int big = 0;
		int medium = 0;
		int small = 0;
		if (3000 <= maxi <= 4250) {
			big = 3; //maximum number of big factories possibly closing
			medium = 10; //maximum number of medium factories possibly closing
		}
		if (1500 <= maxi < 3000) {
			big = 6; //maximum number of big factories possibly closing
			medium = 30; //maximum number of medium factories possibly closing
			small = 10;
		}
		else {
			big = 10; //maximum number of big factories possibly closing
			medium = 60; //maximum number of medium factories possibly closing
			small = 30; //maximum number of small factories possibly closing
		}
		srand((int)time(0));
		double probability1 = (rand() % (big)); //number of big factories closing
		while (probability1 > 0) {
			srand((int)time(0));
			employment -= 100 + (rand() % (120));
			probability1 -= 1;
		}
		double probability2 = (rand() % (medium)); //number of medium factories closing
		while (probability2 > 0) {
			srand((int)time(0));
			employment -= 20 + (rand() % (80));
			probability2 -= 1;
		}
		double probability3 = (rand() % (small)); //number of small factories closing
		while (probability3 > 0) {
			srand((int)time(0));
			employment -= (rand() % (20));
			probability2 -= 1;
		}
		factories = 200 - (probability1 + probability2 + probability3);
		CO2Emission += factories * co2(gen);
	}
	else {
		std::normal_distribution <double> co2(850000, 10000);
		CO2Emission += co2(gen);
	}
	std::normal_distribution <double> mercury(2.3E-5, 1E-6);
	mercuryEmission += (mercury(gen)*factories); 
	std::normal_distribution <double> arsenic(2.4E-5, 1E-6);
	arsenicEmission += (arsenic(gen)*factories); 
	std::normal_distribution <double> cadmium(1.E-5, 5E-7);
	cadmiumEmission += (cadmium(gen)*factories); 
	std::normal_distribution <double> nickel(1.85E-4, 5E-6);
	nickelEmission += (nickel(gen)*factories); 
	std::normal_distribution <double> lead(3.7E-4, 5E-6);
	leadEmission += (lead(gen)*factories); 
	std::normal_distribution <double> so2(4, 0.1);
	SO2Emission += (so2(gen)*factories); 
	std::normal_distribution <double> nh3(5, 0.25);
	NH3Emission += (nh3(gen)*factories); 
	std::normal_distribution <double> nox(5, 0.25);
	NOxEmission += (nox(gen)*factories); 
	std::normal_distribution <double> vocs(7.5, 0.5);
	VOCsEmission += (vocs(gen)*factories);
	std::normal_distribution <double> pm(50, 1.5);
	PMEmission += (pm(gen)*factories);
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
