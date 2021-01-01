#pragma once
#include <core/Godot.hpp>
#include <Node.hpp>
#include <Spatial.hpp>

namespace godot {

	class Game : public Spatial
	{
		GODOT_CLASS(Game, Spatial)

	private:

	public:

		Game();
		~Game();

		static void _register_methods();
		void _ready();
		void _process(float delta);
		Ref<ResourceInteractiveLoader> loader;

	};

}