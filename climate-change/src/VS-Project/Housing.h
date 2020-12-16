#pragma once

#include "obj.h"
#include <core/Godot.hpp>

namespace godot {
    class Housing : public Structure {
        GODOT_SUBCLASS(Housing, Structure)

    public:
        Housing();
        ~Housing();
		int windowCost = 200;
		int windowNumber;
		int solarPanelCost = 1500;
		double numberOfInhabitants = 0;

		double get_satisfaction();
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();

		

		
    };

	class House : public Housing {
		GODOT_SUBCLASS(House, Housing)
	public:
		House();
		~House();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		bool solar_panel(); 
		bool double_glazing(); // improve insulation

		int houseType;
		double houseIncome = 0;
        
	};

	class Building : public Housing {
		GODOT_SUBCLASS(House, Housing)
	public:
		Building();
		~Building();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		bool solar_panel(); 
		bool double_glazing(); // improve insulation
		
		int buildingType;
		double coOwnershipBudget = 0;
	};
}
