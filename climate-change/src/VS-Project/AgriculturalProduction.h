#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include "obj.h"
#include <StaticBody.hpp>

namespace godot {
	class AgriculturalProduction : public Production, public StaticBody {
		GODOT_CLASS(AgriculturalProduction, StaticBody)
	private:

	public:
		AgriculturalProduction();
		~AgriculturalProduction();
		void test();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
}
