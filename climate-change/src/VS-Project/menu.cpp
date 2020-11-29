#include "menu.h"


using namespace godot;


Menu::Menu()
{
	Sidebar = false;
}

Node* Menu::getsidebar()
{
	return get_node("Menu")->get_node("Sidebar");
}


void Menu::_register_methods()
{
	register_method((char*)"_process", &Menu::_process);
	register_method((char*)"_input", &Menu::_input);
	register_method((char*)"_ready", &Menu::_ready);
	register_method((char*)"_on_MenuButton_mouse_entered", &Menu::_on_MenuButton_mouse_entered);
	register_method((char*)"_on_MenuButton_mouse_exited", &Menu::_on_MenuButton_mouse_exited);
}


void Menu::_init(){}

void Menu::_input(Input* e)
{
	if (e->get_class() == "InputEventMouseButton") {
		if (((InputEventMouseButton*)e)->is_pressed()) {   
			Sidebar = (Sidebar == true);
			this->getsidebar()->set("visible", Sidebar);
		}
	}
}


void Menu::_ready()
{
	this->getsidebar()->set("visible", Sidebar);
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
