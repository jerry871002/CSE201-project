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