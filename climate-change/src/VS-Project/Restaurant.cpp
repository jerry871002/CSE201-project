#include "Restaurant.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Restau::_register_methods(){
	register_method((char*)"_process", &Restau::_process);
	register_method((char*)"_input", &Restau::_input);
	register_method((char*)"_ready", &Restau::_ready);
}

void Restau::_init(){

}

void Restau::_process(float delta){

}

void Restau::_input(InputEvent* e){
	if (e->get_class() == "InputEventMouseButton") {
		if (((InputEventMouseButton*)e)->is_pressed()) {   //And button index = ?
			object_selected();
		}
	}
}


void Restau::_ready(){

}


Restau::Restau(){}

Restau::~Restau(){}

void Restau::object_selected() {
	
}	

