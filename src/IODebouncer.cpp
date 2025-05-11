#include "IODebouncer.h"

IODebouncer::IODebouncer(int pin, unsigned long debouncePeriod, bool highIsPressed)
: pin(pin)
, debouncePeriod(debouncePeriod)
, highIsPressed(highIsPressed)
, lastPressed(0)
, lastReleased(0)
, previousPressed(false)
, previousReleased(false)
{
    pinMode(pin, INPUT);
}

bool IODebouncer::isPressed() {
    return is(digitalRead(pin) == highIsPressed ? HIGH : LOW, &lastPressed);
}

bool IODebouncer::isJustPressed() {
    return isJust(isPressed(), &previousPressed);
}

bool IODebouncer::isReleased() {
    return is(digitalRead(pin) == highIsPressed ? LOW : HIGH, &lastReleased);
}

bool IODebouncer::isJustReleased() {
    return isJust(isReleased(), &previousReleased);
}

bool IODebouncer::isOn() {
    return digitalRead(pin);
}

bool IODebouncer::is(bool actualValue, unsigned long * last) {
    if (actualValue) {
        if (*last > 0) {
            return millis() - (*last) >= debouncePeriod;
        } else {
            *last = millis();
        }
    } else {
        *last = 0;
    }
    return false;
}

bool IODebouncer::isJust(bool actualValue, bool * previousValue) {
    if (actualValue) {
        if (*previousValue) {
            return false;
        }
        *previousValue = true;
    } else {
        *previousValue = false;
    }
    return *previousValue;
}