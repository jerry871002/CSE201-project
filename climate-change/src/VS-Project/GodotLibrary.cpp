#include "Player.h"
#include "Restaurant.h"
#include "City.h"
#include "AgriculturalProduction.h"
#include "AllPhysicalGoodsFactories.h"
#include "CoalPowerPlant.h"
#include "Education.h"
#include "Health.h"
#include "HighBuilding.h"
#include "HighHouse.h"
#include "LowBuilding.h"
#include "LowHouse.h"
#include "Mall.h"
#include "NuclearPowerPlant.h"
#include "RecyclingFactory.h"
#include "Services.h"
#include "SmallShop.h"
#include "Windmill.h"



using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options * o) {
	Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options * o) {
	Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle) {
	Godot::nativescript_init(handle);
	register_class<Player>();
	register_class<Restaurant>();
	register_class<City>();
	register_class<AgriculturalProduction>();
	register_class<AllPhysicalGoodsFactories>();
	register_class<CoalPowerPlant>();
	register_class<Education>();
	register_class<Health>();
	register_class<HighHouse>();
	register_class<LowBuilding>();
	register_class<HighBuilding>();
	register_class<LowHouse>();
	register_class<Mall>();
	register_class<NuclearPowerPlant>();
	register_class<RecyclingFactory>();
	register_class<Services>();
	register_class<SmallShop>();
	register_class<Windmill>();
}



