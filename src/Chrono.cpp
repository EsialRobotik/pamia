#include "Chrono.hpp"

Chrono::Chrono(unsigned long period)
: period(period)
, startTime(0)
, nextTriggerTime(0)
, even(false)
{
}

void Chrono::start() {
    startTime = millis();
    nextTriggerTime = startTime + period;
}

void Chrono::reset() {
    startTime = 0;
    nextTriggerTime = 0;
    even = false;
}

bool Chrono::triggered() {
    unsigned long now = millis();
    if (startTime > 0 && now > nextTriggerTime)  {
        while (now > nextTriggerTime) {
            nextTriggerTime += period;
            even = !even;
        }
        return true;
    }
    return false;
}

bool Chrono::isElapsed() {
    return startTime > 0 && millis() > (startTime + period);
}


unsigned long Chrono::getStartTime() {
    return startTime;
}


unsigned long Chrono::getElapsedTime() {
    return startTime == 0 ? 0 : (millis() - startTime);
}

void Chrono::setPeriod(unsigned long period) {
    this->period = period;
}

bool Chrono::isEven() {
    return even;
}