#include "City.h"
#include "Car.h"
#include <SceneTree.hpp>

#include <ResourceLoader.hpp>
#include <Resource.hpp>
#include <ResourceFormatLoader.hpp>
#include <Script.hpp>
#include <ResourcePreloader.hpp>
#include <PackedScene.hpp>
#include <Tree.hpp>
#include <Node.hpp>

#include <stdlib.h>
#include <iostream>


using namespace godot;

City::City() 
{
	
	
}

City::~City() {}

void City::_register_methods()
{
	register_method((char*)"_process", &City::_process);
	register_method((char*)"_input", &City::_input);
	register_method((char*)"_ready", &City::_ready);
	register_method((char*)"_physics_process", &City::_physics_process);
	register_method((char*)"_init", &City::_init);
}

void City::_init() 
{
	ResourceLoader* rl = ResourceLoader::get_singleton();

	Ref<PackedScene> scene = rl->load("res://Ressources/Bugatti.tscn", "PackedScene");

	if (scene.is_valid()) {
		Node* node = scene->instance();
		node->set("scale", Vector3(10, 10, 10));
		node->set("translation", Vector3(10, 10, 10));
		Car* node2 = (Car*)node;
		//std::cout << node2->___get_class_name() << std::endl;
		this->add_child((Node*)node2);
	}

}

void City::_ready() 
{
	
}

void City::_process(float delta)
{
	
}

void City::_physics_process(float delta) {}

void City::_input(InputEvent* e) {}

