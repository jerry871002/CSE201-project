#pragma once

#include "obj.h"
#include <core/Godot.hpp>

/// <summary>
/// SUPER CLASS PRODUCTION
/// </summary>

namespace godot {
	class Production : public Structure {
		GODOT_SUBCLASS(Production, Structure)

	public:
		virtual String class_name();
		Production();
		~Production();

		// Harmful chemicals emitted :

		double SO2Emission = 0; //kg of sulfure dioxide emitted 
		double NOxEmission = 0; //kg of nitrogen oxides emitted 
		double PMEmission = 0; //particulate matter emitted
		double O3Emission = 0; //ozone formed from emitted chemicals
		double COEmission = 0; //kg of carbon monoxide emitted
		double NH3Emission = 0; //kg of ammonia emitted
		double benzene = 0; //kg of benzene released
		double VOCsEmission; // kg of volatile organic compounds emitted
		double CFCsEmission; // kg of Chlorofluorocarbons emitted

		//Heavy metals released (in kg) :
		double arsenic = 0;
		double cadmium = 0;
		double lead = 0;
		double nickel = 0;
	};

	/// <summary>
	/// AGRICULTURAL PRODUCTION
	/// </summary>

	class AgriculturalProduction : public Production {
		GODOT_SUBCLASS(AgriculturalProduction, Production)
	public:
		virtual String class_name();
		AgriculturalProduction();
		~AgriculturalProduction();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};

	/// <summary>
	/// GOODS FACTORIES
	/// </summary>
	
	/* This class models the manufacturing and industry secotrs.
	Its purpose is to simulate the factories/companies in enough detail in order to see a change in pollution, employment and energy usage given policies 
	related to it (taxes, subsidies, charges, prohibitions...) */

	class GoodsFactories : public Production {
		GODOT_SUBCLASS(GoodsFactories, Production)
	public:
		virtual String class_name();
		GoodsFactories();
		~GoodsFactories();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};

	/// <summary>
	/// SERVICES
	/// </summary>

	/* This class models to the following sectors : education and health services, trade, professional and business services, leisure 
	and hospitality, financial activities, public administration, information and other services.
	Its purpose is to model primarly the employment, emissions and energy usage of this sector in an overall way */

	class Services : public Production {
		GODOT_SUBCLASS(Services, Production)
	public:
		virtual String class_name();
		Services();
		~Services();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};

	/// <summary>
	/// RECYCLING FACTORIES
	/// </summary>

	class RecyclingFactories : public Production {
		GODOT_SUBCLASS(RecyclingFactories, Production)
	public:
		virtual String class_name();
		RecyclingFactories();
		~RecyclingFactories();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
	};
}