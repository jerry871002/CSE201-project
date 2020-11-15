#pragma once
#include <core/Godot.hpp>
#include <Object.hpp>


namespace godot {
	class Data : public Object {
		GODOT_CLASS(Data, Object)
	private:
		int value;
	public:

		void set_value(int);
		void change_value(int);
		int get_value();

		Data();
		~Data();
	};
}

