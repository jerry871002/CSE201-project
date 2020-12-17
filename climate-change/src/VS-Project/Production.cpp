#include "Production.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>

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
}

AgriculturalProduction::~AgriculturalProduction() {}

void AgriculturalProduction::simulate_step(double days)
{
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
