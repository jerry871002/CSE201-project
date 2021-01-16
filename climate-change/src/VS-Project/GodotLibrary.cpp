#include "Player.h"
#include "City.h"
#include "Transport.h"
#include "obj.h"
#include "Shop.h"
#include "Housing.h"
#include "Production.h"
#include "Energy.h"


using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options * o) {
	Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options * o) {
	Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle) {
	Godot::nativescript_init(handle);
	register_class<Structure>();
	register_class<Shop>();
	register_class<Transport>();
	register_class<Player>();
	register_class<Restaurant>();
	register_class<City>();
	register_class<Housing>();
	register_class<Energy>();
	register_class<Production>();
	register_class<House>();
	register_class<Windmill>();
	register_class<NuclearPowerPlant>();
	register_class<SmallShop>();
	register_class<Mall>();
	register_class<Pedestrian>();
	register_class<Building>();
	register_class<CoalPowerPlant>();
	register_class<GeothermalPowerPlant>();
	register_class<AgriculturalProduction>();
	register_class<GoodsFactories>();
	register_class<Services>();
	register_class<Pedestrian>();
}



