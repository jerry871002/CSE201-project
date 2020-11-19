#pragma once
#include <core/Godot.hpp>
#include "data.h"

using namespace godot;

Data::Data() {
	value = 0;
	values.insert(this);
	
}

Data::~Data() {
	values.erase(this);
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