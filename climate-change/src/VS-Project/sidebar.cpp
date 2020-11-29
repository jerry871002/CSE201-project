#include "sidebar.h"
#include "City.hpp"
using namespace godot;

void Menu::_register_methods() {
	register_method((char*)"_on_TextureButton_pressed", &Menu::_on_TextureButton_pressed);
}

Menu::Menu() {}
Menu::~Menu() {}
void Menu::_init() {}
void set_item_text ( int 2, String Date:<<return_game_date ){}