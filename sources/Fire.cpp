#include <chrono>
#include "../headers/Fire.h"

Fire::Fire(int duration) {
    durationTime = duration;
    timestamp = chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}