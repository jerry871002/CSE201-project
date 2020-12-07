#include "menu.h"
#include <ItemList.hpp> 
#include <iostream>

using namespace std;
using namespace godot;


Menu::Menu(){};
Menu::~Menu(){};

void Menu::_register_methods()
{
	register_method((char*)"_input", &Menu::_input);
	register_method((char*)"_ready", &Menu::_ready);
	register_method((char*)"_on_MenuButton_mouse_entered", &Menu::_on_MenuButton_mouse_entered);
	register_method((char*)"_on_MenuButton_mouse_exited", &Menu::_on_MenuButton_mouse_exited);
	register_method((char*)"_on_MenuButton_pressed", &Menu::_on_MenuButton_pressed);
}


void Menu::_init(){}


void Menu::_input(Input* e)
{
} 


void Menu::_ready()
{
	Sidebar = false;
	this->get_child(0)->set("visible", Sidebar);
	std::cout << "success" << std::endl;

}

void godot::Menu::_on_MenuButton_mouse_entered()
{
	Input* i = Input::get_singleton();
	i->set_default_cursor_shape(i->CURSOR_POINTING_HAND);
	
}

void godot::Menu::_on_MenuButton_mouse_exited()
{
	Input* i = Input::get_singleton();
	i->set_default_cursor_shape(i->CURSOR_ARROW);
}

void godot::Menu::_on_MenuButton_pressed()
{
	Sidebar = (Sidebar == true);
	std::cout << "success" << std::endl;
	this->get_child(0)->set("visible", Sidebar);
}