#include "City.h"
#include "Car.h"
#include <SceneTree.hpp>
#include <Instance.h>
#include <ResourceLoader.hpp>
#include <Resource.hpp>
#include <ResourceFormatLoader.hpp>

using namespace godot;

City::City() {}

City::~City() {}

void City::_register_methods()
{
	register_method((char*)"_process", &City::_process);
	register_method((char*)"_input", &City::_input);
	register_method((char*)"_ready", &City::_ready);
	register_method((char*)"_process", &City::_physics_process);
	register_method((char*)"_process", &City::_init);
}

void City::_init() 
{
	// load restaurant at position (0,0)
	//Ref<PackedScene> scene = ResourceLoader::load("root://Ressources/Restau.glb", "PackedScene");
	//if (scene.is_valid()) {
	//	Node* node = scene->instance();
	//}
}

void City::_ready() {}

void City::_process(float delta)
{
	
}

void City::_physics_process(float delta) {}

void City::_input(InputEvent* e) {}

