#pragma once
#include <core/Godot.hpp>
#include "data.h"
#include <string>



/* temporary comment out

using namespace godot;




Data::Data() {
	value = 0;
	values.insert(this);
	units = "no units";
}

Data::~Data() {
	values.erase(this);
}

void Data::set_value(float i) {
	value = i;
}

void Data::set_units(string new_units) {
	units = new_units;
}

void Data::change_value(float i) {
	value += i;
}

float Data::get_value() {
	return value;
}

string Data::get_units() {
	return units;
}


Index::Index() {
	indices.insert(this);
	set_value(50);
}

Index::~Index() {
	indices.erase(this);
}

bool Index::is_capped() {
	return (get_value() == 100);
}

*/