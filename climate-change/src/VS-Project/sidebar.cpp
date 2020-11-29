#include "sidebar.h"

using namespace godot;

void Menu::_register_methods() {
	register_method((char*)"_on_TextureButton_pressed", &Menu::_on_TextureButton_pressed);
}

Menu::Menu() {}
Menu::~Menu() {}
void Menu::_init() {}
