#include "Energy.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>
#include "City.h"
#include <random>

using namespace godot;

/// <summary>
/// ENERGY SUPER CLASS
/// </summary>

String godot::Energy::class_name()
{
	return "Energy";
}

Energy::Energy() {

}

Energy::~Energy() {

}

double Energy::get_energy_output() {
	return this->energyOutput;
}

void Energy::set_energy_output(double energyOutput) {
	this->energyOutput = energyOutput;
}

double Energy::get_radiation()
{
	return this->radiation;
}

double Energy::get_PMEemission()
{
	return this->PMEmission;
}

double Energy::get_H2Semission()
{
	return this->H2SEmission;
}

double Energy::get_CH4emission()
{
	return this->CH4Emission;
}

double Energy::get_NH3emission()
{
	return this->NH3Emission;
}

double Energy::get_SO2emission()
{
	return this->SO2Emission;
}

double Energy::get_NOxemission()
{
	return this->NOxEmission;
}

double Energy::get_mercury_emission()
{
	return this->mercuryEmission;
}

double Energy::get_uranium()
{
	return this->naturalUranium;
}

double Energy::get_fissile_material()
{
	return this->fissileMaterial;
}

double Energy::get_coal()
{
	return this->coal;
}


//INFORMATION DISPLAY 

void Energy::_register_methods()
{
	register_property<Energy, double>("energyOutput", &Energy::set_energy_output, &Energy::get_energy_output, 1);
	register_property<Energy, double>("environmentalCost", &Energy::set_environmental_cost, &Energy::get_environmental_cost, 1);
}

template<typename T> String to_godot_string(T s)
{
	std::string standardString = std::to_string(s);
	godot::String godotString = godot::String(standardString.c_str());
	return godotString;
}

String Energy::get_object_info()
{
	String info = this->Structure::get_object_info();

	info += "Age of the building in days: " + to_godot_string((int)(this->get("age"))) + String("\n");
	info += "Employment: " + to_godot_string((int)(this->employment)) + String("\n");
	info += "CO2 Emissions in tons per year: " + to_godot_string((int)(this->get("CO2Emission"))) + String("\n");
	info += "Energy produced in kWh per year: " + to_godot_string((int)(this->get("energyOutput"))) + String("\n");
	info += "Environmental and health costs induced in euros per year: " + to_godot_string((int)this->get("environmentalCost")) + String("\n");
	info += "Satisfaction meter, out of 10: " + to_godot_string((int)(this->get("satisfaction"))) + String("\n");
	return info;
}


/// <summary>
/// NUCLEAR POWERPLANT CLASS
/// </summary>

String godot::NuclearPowerPlant::class_name()
{
	return "NuclearPowerPlant";
}

NuclearPowerPlant::NuclearPowerPlant() {
	std::random_device rd;
	std::mt19937 gen(rd());

	age = 0;

	radiation = 1.4E-12; //amount of radiation in millirem per kWh per person living in a 50km radius from the plant 
	//Just for perspective, coal plants actually release more radiation and an average person receives an exposure of
	//300 millirem per year from natural background sources of radiation
	nuclearWaste = 3.4E-9; //tons of high level nuclear waste produced per kWh (it is stored to be reused when radiation decays)
	naturalUranium = 3.4E-8; //tons of natural uranium necessary per kWh
	fissileMaterial = 1.4E-10; //tons of fissile material needed per kWh
	CO2Emission = 0.012E-3; // tons of CO2 emitted per kWh

	maintenance = 0; //maintenace and working cost in euros per year
	environmentalCost = 0; // environmental and health costs in euros per year

	employment = 0; 
	std::normal_distribution <double> sat(2, 0.1);
	satisfaction = sat(gen); // on scale of 10
	std::normal_distribution <double> money(10E9, 10E6);
	cost = money(gen); // cost in euros to build a new plant
	std::normal_distribution <double> build(5, 1);
	buildingTime = build(gen); // years needed to build a new plant
}

NuclearPowerPlant::~NuclearPowerPlant() {}

void NuclearPowerPlant::simulate_step(double days)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	int working = this->get_tree()->get_root()->get_node("Main/3Dworld")->get("workingPower");
	if (working == 1) {
		running = 1;
		newBuilt = false;
		age += (int)(days);
		this->get_node("Smoke")->set("visible", true);
		std::normal_distribution <double> employees(800, 50);
		employment = (int)(employees(gen));
	}
	else {
		running = 0;
		this->get_node("Smoke")->set("visible", false);
	}

	double coal_prohibited = this->get("coal_prohibited"); // input from user
	double nuclear_prohibited = this->get("nuclear_prohibited"); // input from user

	double cityPowerDemand = this->get_tree()->get_root()->get_node("Main/3Dworld")->get("energyDemand");
	double dailyDemand = cityPowerDemand / 365;
	std::normal_distribution <double> energy(dailyDemand, 1000);
	energyPerDay = energy(gen)*running; //kWh produced by standard plant in one day, we consider it to be the same for every plant in our simulation

	if (nuclear_prohibited == 1 || age >= 127890) {
		// 35 years is the average lifetime of a nuclear power plant, it then has to be replaced by a new plant or different power plant
		age = 0;
		energyPerDay = 0; //forced closure of the plant
		employment = 0;
		this->get_node("Smoke")->set("visible", false);
		if (newBuilt == false) {
			newBuilt = true;
			srand((int)time(0));
			double probability = (rand() % (10));
			if (probability <= 8 && coal_prohibited == 0) {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 0);
			}
			else {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 2);
			}
		}
	}

	energyOutput = (int)(energyPerDay * 365); // total kWh produced by a standard plant per year

	fissileMaterial = 1.4E-7 * energyPerDay * 365;
	naturalUranium = 3.4E-8 * energyPerDay * 365;
	CO2Emission = 0.012E-3 * energyPerDay * 365; //ton per year
	nuclearWaste = 3.4E-9 * energyPerDay * 365;
	radiation = 1.4E-12 * energyPerDay * 365;
	environmentalCost = 0.019 * energyPerDay * 365;
	maintenance = 0.04 * energyPerDay * 365;
	if (age >= 3650) {
		maintenance *= 0.25; // after 10 years the maintenance and working costs increase by 1/4
	}
	if (age >= 10950) {
		maintenance *= 2; // after 30 years the maintenance and working costs double
	}
}

void godot::NuclearPowerPlant::_process(float delta)
{
	this->Structure::_process(delta); 
	this->get_node("Smoke")->get_child(0)->set("speed_scale", int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed")));
	this->get_node("Smoke")->get_child(1)->set("speed_scale", int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed")));
}

/// <summary>
/// WINDMILL CLASS
/// </summary>

String godot::Windmill::class_name()
{
	return "Windmill";
}

Windmill::Windmill() {
	std::random_device rd;
	std::mt19937 gen(rd());
	turnSpeed = 1;

	age = 0;
	maintenance = 0; //maintenace and working cost in euros per kWh
	employment = 0; // average number of employees for one windmill
	std::normal_distribution <double> sat(8, 0.1);
	satisfaction = sat(gen); // on scale of 10
	CO2Emission = 0.011E-3; // tons of CO2 emitted per kWh
	std::normal_distribution <double> money(4E6, 300000);
	cost = money(gen); // cost in euros to build a new windmill
	std::normal_distribution <double> build(0.04, 0.005);
	buildingTime =build(gen); // years needed to build a new windmill (approximatley half a month)
	environmentalCost = 0; // environmental and health costs in euros per kWh
	requiredLand = 6070; // square meters of land needed for installing one windmill
}

Windmill::~Windmill() {}

void Windmill::_process(float delta)
{
	this->Structure::_process(delta);
	int SPEED = 0;
	if ((bool)running) {
		SPEED = int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed"));
	}
	rot -= delta * turnSpeed * SPEED;
	((Mesh*)this->get_node("MeshComponents/Blades"))->set("rotation_degrees", Vector3(0, -130, double((180 / 3.1415926535) * rot)));
}

void Windmill::simulate_step(double days)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	int working = this->get_tree()->get_root()->get_node("Main/3Dworld")->get("workingPower");
	if (working == 2) {
		running = 1;
		newBuilt = false;
		age += (int)(days);
		std::normal_distribution <double> employees(1.29, 0.02);
		employment = (int)(employees(gen));
	}
	else {
		running = 0;
	}

	double coal_prohibited = this->get("coal_prohibited"); // input from user
	double nuclear_prohibited = this->get("nuclear_prohibited"); // input from user

	std::normal_distribution <double> energy(25000, 5000);
	energyPerDay = energy(gen)*running; //kWh produced by a standard windmill in one day (average size of 2.5MW windmill)
	
	if (age >= 7300) {
		// 20 years is the average lifetime of a windmill, it then has to be replaced by a new one or destroyed
		age = 0; //set to 0 again
		energyPerDay = 0;
		employment = 0;
		if (newBuilt == false) {
			newBuilt = true;
			srand((int)time(0));
			double probability = (rand() % (10));
			if (nuclear_prohibited == 0 && coal_prohibited == 0) {
				if (probability <= 7) {
					this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 0);
				}
				else {
					this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 1);
				}
			}
			if (nuclear_prohibited == 1 && coal_prohibited == 0) {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 0);
			}
			if (nuclear_prohibited == 0 && coal_prohibited == 1) {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 1);
			}
			else {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 2);
			}
		}
	}

	energyOutput = (int)(energyPerDay * 365); // total kWh produced by a standard plant 

	CO2Emission = 0.011E-3 * energyPerDay * 365;
	environmentalCost = 0.0009 * energyPerDay * 365;
	maintenance = 7.45E-4 * energyPerDay * 365;
}

/// <summary>
/// GEOTHERMAL POWERPLANT CLASS
/// </summary>

String godot::GeothermalPowerPlant::class_name()
{
	return "GeothermalPowerPlant";
}

GeothermalPowerPlant::GeothermalPowerPlant()
{
	srand((int)time(0));
	double probability = (rand() % (10));
	if (probability < 2) {
		cost = 7E6; // when drilling the wells (accounts for 2 million euros approx) there is a 20% chance of failure 
	}
	else {
		cost = 5E6; // cost in euros to build a new plant (plus all the research needed)
	}
	age = 0;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution <double> sat(8, 0.1);
	satisfaction = sat(gen); // on scale of 10
	std::normal_distribution <double> build(8, 1);
	buildingTime = build(gen); // years needed to build a new plant (plus research needed)
	employment = 0; // average number of employees linked to one plant

	CO2Emission = 0.09E-3; // tons of CO2 emitted per kWh
	H2SEmission = 8.2E-8; //tons of H2S emitted per kWh
	CH4Emission = 1.6E-8; //tons of CH4 emitted per kWh
	NH3Emission = 1.7E-8; //tons of NH3 emitted per kWh
	maintenance = 0; //maintenace and working cost in euros per kWh
	environmentalCost = 0; // environmental and health costs in euros per kWh
}

GeothermalPowerPlant::~GeothermalPowerPlant() {}

void godot::GeothermalPowerPlant::_process(float delta)
{
	this->Structure::_process(delta);
	this->get_node("Smoke")->get_child(0)->set("speed_scale", int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed")));
	this->get_node("Smoke")->get_child(1)->set("speed_scale", int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed")));
}

void GeothermalPowerPlant::simulate_step(double days)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	int working = this->get_tree()->get_root()->get_node("Main/3Dworld")->get("workingPower");
	if (working == 2) {
		running = 1;
		newBuilt = false;
		age += (int)(days);
		this->get_node("Smoke")->set("visible", true);
		std::normal_distribution <double> employees(1.29, 0.02);
		employment = (int)(employees(gen));
	}
	else {
		running = 0;
		this->get_node("Smoke")->set("visible", false);
	}

	double coal_prohibited = this->get("coal_prohibited"); // input from user
	double nuclear_prohibited = this->get("nuclear_prohibited"); // input from user

	std::normal_distribution <double> energy(32800, 1500);
	energyPerDay = energy(gen)*running; //kWh produced by in one day
	
	if (age >= 10950) {
		// 20 years is the average lifetime of a windmill, it then has to be replaced by a new one or destroyed
		age = 0; //set to 0 again
		energyPerDay = 0;
		employment = 0;
		if (newBuilt == false) {
			newBuilt = true;
			srand((int)time(0));
			double probability = (rand() % (10));
			if (nuclear_prohibited == 0 && coal_prohibited == 0) {
				if (probability <= 7) {
					this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 0);
				}
				else {
					this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 1);
				}
			}
			if (nuclear_prohibited == 1 && coal_prohibited == 0) {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 0);
			}
			if (nuclear_prohibited == 0 && coal_prohibited == 1) {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 1);
			}
			else {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 2);
			}
		}
	}
	
	energyOutput = (int)(energyPerDay * 365); // total kWh produced by a standard plant 

	maintenance = 0.08 * energyPerDay * 365;
	if (age >= 3650) {
		maintenance = 0.08 * 0.25; // after 10 years the maintenance and working costs increase by 1/4
	}
	if (age >= 7300) {
		maintenance = 0.08; // after 20 years the maintenance and working costs double
	}
	CO2Emission = 0.09E-3 * energyPerDay * 365;
	NH3Emission = 1.7E-8 * energyPerDay * 365;
	CH4Emission = 1.6E-8 * energyPerDay * 365;
	H2SEmission = 8.2E-8 * energyPerDay * 365;

	environmentalCost = 0.0015 * energyPerDay * 365;
}


/// <summary>
/// COAL POWERPLANT CLASS
/// </summary>

String godot::CoalPowerPlant::class_name()
{
	return "CoalPowerPlant";
}

CoalPowerPlant::CoalPowerPlant()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	
	age = 0; // age of the building in days

	std::normal_distribution <double> money(3E9, 10E6);
	cost = money(gen); // cost in euros to build a new plant
	std::normal_distribution <double> build(8, 1);
	buildingTime = build(gen); // years needed to build a new plant
	std::normal_distribution <double> employees(800, 20);
	employment = (int)(employees(gen)); // approximate number of employees in 1 plant 
	std::normal_distribution <double> sat(4, 0.1);
	satisfaction = sat(gen); // on scale of 10

	CO2Emission = 0.868E-3; // tons of CO2 emitted per kWh
	SO2Emission = 0.00152E-3; // tons of SO2 emitted per kWh
	NOxEmission = 8.49E-7; // tons of NOx emitted per kWh
	PMEmission = 4E-8; //tons of total PM emitted per kWh
	ashOutput = 0.0619E-3; // tons of ash produced per kWh
	mercuryEmission = 1.137E-11; // tons of mercury emitted per kWh
	coal = 4.06E-4; // tons of coal needed to produce 1 kWh
	maintenance = 0.05; //maintenace and working cost in euros per kWh
	environmentalCost = 0.06; // environmental and health costs in euros per kWh
}


CoalPowerPlant::~CoalPowerPlant()
{

}

void godot::CoalPowerPlant::_process(float delta)
{
	this->Structure::_process(delta);
	this->get_node("Smoke")->get_child(0)->set("speed_scale", int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed")));
	this->get_node("Smoke")->get_child(1)->set("speed_scale", int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed")));
}

void CoalPowerPlant::simulate_step(double days)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	int working = this->get_tree()->get_root()->get_node("Main/3Dworld")->get("workingPower");
	if (working == 0) {
		running = 1;
		newBuilt = false;
		age += (int)(days);
		this->get_node("Smoke")->set("visible", true);

		std::normal_distribution <double> employees(800, 20);
		employment = (int)(employees(gen));
	}
	else {
		running = 0;
		this->get_node("Smoke")->set("visible", false);
	}

	double efficiency_supercritical = this->get("efficiency_supercritical"); // input from user
	double efficiency_cogeneration = this->get("efficiency_cogeneration"); // input from user
	double coal_prohibited = this->get("coal_prohibited"); // input from user
	double nuclear_prohibited = this->get("nuclear_prohibited"); // input from user

	double cityPowerDemand = this->get_tree()->get_root()->get_node("Main/3Dworld")->get("energyDemand");
	double dailyDemand = cityPowerDemand / 365;
	std::normal_distribution <double> energy(dailyDemand, 1000);
	energyPerDay = energy(gen)*running; //kWh produced by standard plant in one day, we consider it to be the same for every plant in our simulation

	if (coal_prohibited == 1 || age >= 18250) {
		// 50 years is the average lifetime of a coal fired plant, it then has to be replaced by a new coal plant or different power plant
		energyPerDay = 0; //forced closure of the plant
		employment = 0;
		age = 0;
		if (newBuilt == false) {
			newBuilt = true;
			srand((int)time(0));
			double probability = (rand() % (10));
			if (probability <= 8 && nuclear_prohibited == 0) {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 1);
			}
			else {
				this->get_tree()->get_root()->get_node("Main/3Dworld")->set("workingPower", 2);
			}
		}
	}
	
	energyOutput = (int)(energyPerDay * 365); // total kWh produced by a standard plant 
	bool efficiencySup = true;
	bool efficiencyCo = true;
	
	if (efficiency_supercritical == 1) {
		if (efficiencySup == true) {
			maintenance += 10E6;
			efficiencySup = false;
		}
		energyPerDay = dailyDemand - (dailyDemand*0.04);
		maintenance = 0.054 * energyPerDay * 365;
	}
	if (efficiency_cogeneration == 1) {
		if (efficiencyCo == true) {
			maintenance = 20E6;
			efficiencyCo = false;
		}
		energyPerDay = dailyDemand - (dailyDemand * 0.09);
		maintenance = 0.058 * energyPerDay * 365;
	}
	else {
		maintenance = 0.05 * energyPerDay * 365;
	}
	if (age >= 3650) {
		maintenance = 0.05 * 0.25; // after 10 years the maintenance and working costs increase by 1/4
	}
	if (age >= 10950) {
		maintenance = 0.05; // after 30 years the maintenance and working costs double
	}

	coal = 4.06E-4 * energyPerDay * 365;
	CO2Emission = 0.868E-3 * energyPerDay * 365;
	SO2Emission = 0.00152E-3 * energyPerDay * 365;
	NOxEmission = 8.49E-7 * energyPerDay * 365;
	PMEmission = 4E-8 * energyPerDay * 365;
	ashOutput = 0.0619E-3 * energyPerDay * 365;
	mercuryEmission = 1.137E-11 * energyPerDay * 365;
	environmentalCost = 0.06 * energyPerDay * 365;
}
