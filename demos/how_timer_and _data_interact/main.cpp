#include <iostream>
#include "data.h"

void show_status(int time, Indices happiness, Indices economics);
void show_policies();

int main(int argc, char const *argv[]) {
    Indices happiness = Indices(50);
    Indices economics = Indices(50);

    int time = 0;

    while (true) {
        show_status(time, happiness, economics);
        show_policies();

        int policy;
        std::cin >> policy;

        switch (policy) {
            case 1:
                happiness.increase(10);
                economics.decrease(5);
                break;
            case 2:
                happiness.increase(5);
                economics.decrease(10);
                break;
            case 3:
                happiness.decrease(10);
                economics.increase(5);
                break;
            case 4:
                happiness.decrease(5);
                economics.increase(10);
                break;
        };

        time++;
    }
    return 0;
}

void show_status(int time, Indices happiness, Indices economics) {
    std::cout << "============================\n";
    std::cout << "Current time: " << time << std::endl;
    std::cout << "Happiness: " << happiness.get_current_value() << std::endl;
    std::cout << "Economics: " << economics.get_current_value() << std::endl;
    std::cout << "============================\n";
}

void show_policies() {
    std::cout << "1. Do some policy...1" << std::endl;
    std::cout << "2. Do some policy...2" << std::endl;
    std::cout << "3. Do some policy...3" << std::endl;
    std::cout << "4. Do some policy...4" << std::endl;
    std::cout << "Pick a policy: ";
}
