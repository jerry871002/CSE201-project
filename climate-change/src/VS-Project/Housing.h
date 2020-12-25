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
		int maxIncome=333; //max income per day, should be defined by sim team
		int minIncome=53; // min income per day, I put values for now so that it compiles
		int solarPanelAge = 0;
		int doubleGlazingAge = 0;
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
		bool roof_turbines();

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
		bool roof_turbines();
		
		int buildingType;
		double coOwnershipBudget = 0;
	};
}













// #pragma once

// #include "obj.h"
// #include <core/Godot.hpp>



// namespace godot {
//     class Housing : public Structure {
//         GODOT_SUBCLASS(Housing, Structure)

//     public:
// 		virtual String class_name();
//         Housing();
//         ~Housing();
//     };

// 	class House : public Housing {
// 		GODOT_SUBCLASS(House, Housing)
// 	public:
// 		virtual String class_name();
// 		House();
// 		~House();

// 		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)

// 		double numberOfInhabitants = 0;
// 		double houseIncome = 0;
// 	};
// }