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
