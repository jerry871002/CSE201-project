#pragma once
#include <core/Godot.hpp>
#include "data.h"

using namespace godot;

Data::Data() {
	value = 0;
}

Data::~Data() {

}

void Data::set_value(int i) {
	value = i;
}

void Data::change_value(int i) {
	value += i;
}

int Data::get_value() {
	return value;
}
