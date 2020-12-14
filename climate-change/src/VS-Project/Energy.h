#pragma once

#include "obj.h"
#include <core/Godot.hpp>

namespace godot {

	/// <summary>
	/// SUPER CLASS ENERGY
	/// </summary>

	class Energy : public Structure {
		GODOT_SUBCLASS(Energy, Structure)

	public:
		Energy();
		~Energy();

		//inputs:
		//Nuclear plant :
		double naturalUranium = 0;
		double fissileMaterial = 0;
		double requiredLand = 0;
		double coal = 0;
		//outputs:
		double nuclearWaste = 0;
		double radiation = 0;
		double energyPerDay = 0;
		double energyOutput = 0;
		double H2SEmission = 0;
		double CH4Emission = 0;
		double NH3Emission = 0;
		double SO2Emission = 0;
		double NOxEmission = 0;
		double ashOutput = 0;
		double mercuryEmission = 0;

	};

	/// <summary>
	/// NUCLEAR POWER PLANT CLASS
	/// </summary>

	class NuclearPowerPlant : public Energy {
		GODOT_SUBCLASS(NuclearPowerPlant, Energy)
	public:
		NuclearPowerPlant();
		~NuclearPowerPlant();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};

	/// <summary>
	/// WINDMILL CLASS
	/// </summary>

	class Windmill : public Energy {
		GODOT_SUBCLASS(Windmill, Energy)
	public:
		Windmill();
		~Windmill();

		void _process(float delta);

		//This class gives the functionnalities for one single onshore windmill, 
		//the sim team can use it in order to simulate a wind farm of the size it chooses
		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		double turnSpeed;
	private:
		double rot = 0;
	};

	/// <summary>
	/// COAL POWER PLANT CLASS
	/// </summary>

	class CoalPowerPlant : public Energy {
		GODOT_SUBCLASS(CoalPowerPlant, Energy)
	public:
		CoalPowerPlant();
		~CoalPowerPlant();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};

	/// </summary>
	/// GEOTHERMAL POWER PLANT CLASS
	/// </summary>

	class GeothermalPowerPlant : public Energy {
		GODOT_SUBCLASS(GeothermalPowerPlant, Energy)
	public:
		GeothermalPowerPlant();
		~GeothermalPowerPlant();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};
}