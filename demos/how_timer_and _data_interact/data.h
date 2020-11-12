class Data {

};

class Indices: public Data {
public:
    Indices(int base_value): current_value(base_value) {}
    void increase(int amount) { current_value += amount; }
    void decrease(int amount) { current_value -= amount; }
    int get_current_value() { return current_value; }
private:
    static const int max = 100;
    static const int min = 0;
    int current_value;
};

class Counter: public Data {

};