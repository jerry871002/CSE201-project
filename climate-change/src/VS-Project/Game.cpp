#include "Game.h"
#include <stdlib.h>
#include <iostream>
#include <ResourceLoader.hpp>
#include <Resource.hpp>
#include <PackedScene.hpp>
#include <ResourceInteractiveLoader.hpp>

using namespace godot;

Game::Game() {};

Game::~Game() {};

void Game::_register_methods() {

	register_method((char*)"_ready", &Game::_ready);
	register_method((char*)"_process", &Game::_process);
}

void Game::_ready() {
	std::cout << "THE GAME IS READY!" << std::endl;
	/*
	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	Ref<PackedScene> Main3DScene = ResLo->load("res://Resources/3Dworld.tscn", "PackedScene");
	if (Main3DScene.is_valid())
	{
		Node* node = Main3DScene->instance();
		node->set("visible", true);
		this->add_child(node);
	}
	*/

	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	loader = ResLo->load_interactive("res://Resources/3Dworld.tscn");

	/*
	int count = Main3DScene->get_stage_count();

	Godot::print(count);

	std::cout << "The number of stages to load the 3D world is: " << Main3DScene->get_stage_count() << std::endl;

	
	while (Main3DScene->get_stage() != count) {
		Main3DScene->poll();
		std::cout << "STAGE " << Main3DScene->get_stage() << " OF " << count << " COMPLETED" << std::endl;
	}
	*/


}

void Game::_process(float delta) {

}