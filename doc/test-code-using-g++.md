# Test the objects and algorithms using `g++` on mac

This article is mainly for mac users in the object and algorithm team, which in some scenario, we wish to test our object or algorithm one by one instead of making the dynamic library and view it on Godot.

A step-by-step example is shown below to demostrate how to write test code, compile them into executable, and execute them to see the result.

## Step 1: Choose the object you want to test

Say we want to test the `Restaurant` object and see if the newly create function `get_total_salary()` works properly
> Just an example, we don't actually have `get_total_salary()` in our code on GitHub

**Restaurant.h**

```cpp=
#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include "obj.h"

namespace godot {
    class Restaurant : public Struc, public StaticBody {
        GODOT_CLASS(Restaurant, StaticBody)
    private:
        int totalEmployee;
        int averageSalary;
    public:
        Restaurant();
        Restaurant(int totalEmp, avgSalary);
        
        // CODE TO TEST!!
        int get_total_salary();

        // Godot related stuff
        static void _register_methods();
        void _init();
        void _process(float delta);
        void _input(InputEvent* e);
        void _ready();
        void _on_Area_mouse_entered();
        void _on_Area_mouse_exited();    
    };
}
```

**Restaurant.cpp**

```cpp=
#include "Restaurant.h"

using namespace godot;

Restaurant::Restaurant() {
    totalEmployee = 10;
    averageSalary = 1000;
}

Restaurant::Restaurant(int totalEmp, avgSalary) {
    totalEmployee = totalEmp;
    averageSalary = avgSalary; 
}

int Restaurant::get_total_salary() {
    return totalEmployee * averageSalary;
}
```

## Step 2: Write test code

Create a file called `test_restaurant.cpp` (in the same folder as `Restaurant.h` and `Restaurant.cpp`). We will write our test code here.

**test_restaurant.cpp**

```cpp=
#include <iostream>
#include "Restaurant"

void test_get_total_salary();

int main() {
    test_get_total_salary();
}

void test_get_total_salary() {
    // normal restaurant
    Restaurant r1 = Restaurant();
    // fancy restaurant
    Restaurant r2 = Restaurant(15, 1250);
    
    std::cout << "Restaurant 1:" << std::endl;
    std::cout << "Expected result: " << (10 * 1000) << std::endl;
    std::cout << "Actual result: " << r1.get_total_salary() << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Restaurant 2:" << std::endl;
    std::cout << "Expected result: " << (15 * 1250) << std::endl;
    std::cout << "Actual result: " << r2.get_total_salary() << std::endl;
}
```

You can write as many test functions in this file as you need, in this way we can test different functionality in the same object with the same file.

## Step 3: Compile the test code with the object code

Since we are testing the `Restaurant` object, the file we need is `test_restaurant.cpp` and `Restaurant.cpp` (and also `obj.cpp` since our `Restaurant` object inherit from object in this file)

### Files to compile (no need to compile header files!)

* test_restaurant.cpp
* Restaurant.cpp
* obj.cpp

---

Next we have to tell the compiler where all the Godot related things are, first let's start with the headers, the headers are in these four locations

### Header files (we use relative path with respect to the code)

* Libraries/godot-cpp-bindings/godot_headers 
* Libraries/godot-cpp-bindings/include 
* Libraries/godot-cpp-bindings/include/core 
* Libraries/godot-cpp-bindings/include/gen

---

Lasting, we have to tell the compiler where the godot library is

### Godot Library

* Libraries/godot-cpp-bindings/bin
    > In this folder
* godot-cpp.osx.debug.64.a
    > with this name (if you use windows it's `libgodot-cpp.windows.debug.64.lib`)

---

Let's combine them all together with `g++` syntax. First open a terminal and `cd` to the folder that contains the code

> Hint: `g++` syntax
> * `-I` for telling the header files' path
> * `-L` for telling the path that contains the library
> * `-l` for telling the name of the library
> * `-o` for naming the executable
> * `-std` for telling which C++ version are we using

Type the following command in the terminal

```bash
g++ -std=c++17 test_restaurant.cpp Restaurant.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers -ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core -ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin -lgodot-cpp.osx.debug.64 -o test_restaurant
```

If there is no error occurs, the executable is successfully generated.

## Step 4: Execute it and check the result

Execute it by typing the following
> The name after `-o` when you compile!

```bash
./test_restaurant
```

It will display

```
Restaurant 1:
Expected result: 10000
Actual result: 10000

Restaurant 2:
Expected result: 18750
Actual result: 18750
```

Seems everything works well, at this point, the `get_total_income()` function is verified to be correct.
