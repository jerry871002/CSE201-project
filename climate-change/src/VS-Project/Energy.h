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
		virtual String class_name();
		Energy();
		~Energy();

		virtual String get_object_type() { return "Energy"; };
		String get_main_type() { return "Energy"; };

		static void _register_methods();

		virtual String get_object_info();

		//inputs::
		double naturalUranium = 0;
		double fissileMaterial = 0;
		double requiredLand = 0;
		double coal = 0;
		//outputs:
		double energyPerDay = 0;
		double energyOutput = 0;

		double H2SEmission = 0;
		double CH4Emission = 0;
		double NH3Emission = 0;
		double SO2Emission = 0;
		double NOxEmission = 0;
		double PMEmission = 0;

		double radiation = 0;
		double mercuryEmission = 0;

		double ashOutput = 0;
		double nuclearWaste = 0;

		virtual double get_energy_output();
		virtual void set_energy_output(double energyOutput);

		//All emissions caused by the plants which have a negative impact on the environment 
		//(I am not having you deal with the waste such as ash for the moment)
		virtual double get_radiation();
		virtual double get_PMEemission();
		virtual double get_H2Semission();
		virtual double get_CH4emission();
		virtual double get_NH3emission();
		virtual double get_SO2emission();
		virtual double get_NOxemission();
		virtual double get_mercury_emission();

		//Inputs needed for the different plants to work (important for the econ simulation aspect)
		virtual double get_uranium();
		virtual double get_fissile_material();
		virtual double get_coal();

		//Variables for overall energy simulation : 
		bool newBuilt = false;
		int running;
	};

	/// <summary>
	/// NUCLEAR POWER PLANT CLASS
	/// </summary>

	class NuclearPowerPlant : public Energy {
		GODOT_SUBCLASS(NuclearPowerPlant, Energy)
	public:
		virtual String class_name();
		NuclearPowerPlant();
		~NuclearPowerPlant();
		virtual String get_object_type() { return "Nuclear Power Plant"; };

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)

		void _process(float delta);
	};

	/// <summary>
	/// WINDMILL CLASS
	/// </summary>

	class Windmill : public Energy {
		GODOT_SUBCLASS(Windmill, Energy)
	public:
		virtual String class_name();
		Windmill();
		~Windmill();
		virtual String get_object_type() { return "Windmill"; };

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
		virtual String class_name();
		CoalPowerPlant();
		~CoalPowerPlant();
		virtual String get_object_type() { return "Coal Power Plant"; };

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)

		void _process(float delta);
	};

	/// </summary>
	/// GEOTHERMAL POWER PLANT CLASS
	/// </summary>

	class GeothermalPowerPlant : public Energy {
		GODOT_SUBCLASS(GeothermalPowerPlant, Energy)
	public:
		virtual String class_name();
		GeothermalPowerPlant();
		~GeothermalPowerPlant();
		virtual String get_object_type() { return "Geothermal Power Plant"; };

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)

		void _process(float delta);
	};
}