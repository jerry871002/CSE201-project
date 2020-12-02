#include "City.h"
#include "Car.h"
#include "Restaurant.h"
#include <SceneTree.hpp>

#include <ResourceLoader.hpp>
#include <Resource.hpp>
#include <ResourceFormatLoader.hpp>
#include <Script.hpp>
#include <ResourcePreloader.hpp>
#include <PackedScene.hpp>
#include <Tree.hpp>
#include <Node.hpp>

#include <cstdlib>
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
	

}

void City::_ready() 
{
	std::cout << "HELLO" << std::endl;
	ResourceLoader* ResLo = ResourceLoader::get_singleton();

	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");

	if (RestaurantScene.is_valid() && ShopScene.is_valid())
	{
		for (int x = 0; x < 5; x ++)
		{
			for (int z = 0; z < 5; z ++)
			{
				int type = rand() % 2;
				Node* node;
				if (type == 0) { node = RestaurantScene->instance(); } else { node = ShopScene->instance(); }
				node->set("scale", Vector3(10, 10, 10));
				node->set("translation", Vector3(30 * x, 0, 30 * z));
				int rot = rand() % 2;
				node->set("rotation_degrees", Vector3(0, 180 * rot, 0));
				this->add_child(node);
			}
		}
	}

	if (BugattiScene.is_valid())
	{
		Node* node = BugattiScene->instance();
		node->set("scale", Vector3(10, 10, 10));
		node->set("translation", Vector3(0, 0, 0));
		this->add_child(node);
	}
}

void City::_process(float delta)
{
	
}

void City::_physics_process(float delta) {}

void City::_input(InputEvent* e) {}

