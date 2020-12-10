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
	register_method((char*)"_on_CloseButton_pressed", &Menu::_on_CloseButton_pressed);
	register_method((char*)"_on_Time1_pressed", &Menu::_on_Time1_pressed);
	register_method((char*)"_on_Time2_pressed", &Menu::_on_Time2_pressed);
	register_method((char*)"_on_GraphButton_pressed", &Menu::_on_GraphButton_pressed);

}


void Menu::_init(){}


void Menu::_input(Input* e)
{
} 


void Menu::_ready()
{
	Sidebar = false;
	this->get_child(1)->set("visible", Sidebar);
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
	this->get_child(1)->set("visible", !Sidebar);
	Sidebar = !Sidebar;
}

void godot::Menu::_on_CloseButton_pressed()
{
	this->get_child(1)->set("visible", !Sidebar);
	Sidebar = !Sidebar;
}


void godot::Menu::_on_Time1_pressed()
{
	this->get_child(1)->set("visible", !Sidebar);
	Sidebar = !Sidebar;
}

void godot::Menu::_on_Time2_pressed()
{
	this->get_child(1)->set("visible", !Sidebar);
	Sidebar = !Sidebar;
}

void godot::Menu::_on_GraphButton_pressed()
{
	this->get_child(1)->set("visible", !Sidebar);
	Sidebar = !Sidebar;
}