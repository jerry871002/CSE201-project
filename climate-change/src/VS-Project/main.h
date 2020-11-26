#include <iostream>

//implementation running with set fps, not ideal but easy to work with
//working with ticks as "turns", can achieve a "continuous time"

const int FRAMES_PER_SECOND=25; //or let the value be set by user
const int TICKS_PER_FRAME = 1000/FRAMES_PER_SECOND; //1s=1000ms

double num_ticks = TickCount(); //returns number of milliseconds since game started

bool running = true;
int time = 0;
int down_time = 0;

while(running) {
    update_game();
    display_game();

    num_ticks += TICKS_PER_FRAME;
    down_time = num_ticks - TickCount();
    if (down_time >= 0) {
        Sleep(down_time);
    }
    else {
        //too bad...
        //decrease fps
    }
};

/* implementation using godot main scene:
bool running = true;
while(running) {
    update_game(); //based on physics_process function of godot (e.g. update data once per second)
    display_game(); //based on godot game engine, thus fps
}
*/