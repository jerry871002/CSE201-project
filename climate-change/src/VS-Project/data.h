#pragma once
#include <core/Godot.hpp>
#include <Object.hpp>


// created a Data class. this class has a value, which can be edited, a constructor which sets value to 0 and a virtual update_data() method

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
		update_data();
	};
}
