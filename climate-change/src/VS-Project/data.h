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
		virtual void update_statistic();
		void set_value(int);
		void change_value(int);
		int get_value();

		Data();
		~Data();
		
	};
}
////Counters
namespace godot {
	class Income : public Data {
		GODOT_CLASS(Income, Data)
	private:
		
	public:

		
	};
}

namespace godot {
	class Population : public Data {
		GODOT_CLASS(Population, Data)
	private:

	public:


	};
}


namespace godot {
	class Employed : public Data {
		GODOT_CLASS(Employed, Data)
	private:

	public:


	};
}


namespace godot {
	class Carbon : public Data {
		GODOT_CLASS(Carbon, Data)
	private:

	public:


	};
}


namespace godot {
	class EnergyDemand : public Data {
		GODOT_CLASS(EnergyDemand, Data)
	private:

	public:


	};
}

namespace godot {
	class EnergySupply : public Data {
		GODOT_CLASS(EnergySupply, Data)
	private:

	public:


	};
}

namespace godot {
	class Needs : public Data {
		GODOT_CLASS(Needs, Data)
	private:

	public:


	};
}




////Indices

// added a cap - the bool cap returns true when the value is at 100 
// incomplete ! need to add a function to update

namespace godot {
	class Index : public Data {
		GODOT_CLASS(Index, Data)
	private:
		bool capped;

	public:

		bool is_capped();

	};
}

namespace godot {
	class Happiness : public Index {
		GODOT_CLASS(Happiness,Index)
	



	};


}



namespace godot {
	class Employment : public Index {
		GODOT_CLASS(Employment, Index)




	};


}



namespace godot {
	class Emissions : public Index {
		GODOT_CLASS(Emissions, Index)




	};


}



namespace godot {
	class Medical : public Index {
		GODOT_CLASS(Medical, Index)




	};


}

namespace godot {
	class Employment : public Index {
		GODOT_CLASS(Employment, Index)




	};


}
