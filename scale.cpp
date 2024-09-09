#include "scale.h"

void Scale::init() {
    scale.begin(PIN_SCALE_DATA, PIN_SCALE_SCK);
    scale.set_raw_mode();
}

int Scale::getWeightPercentage() {
    float value = scale.get_value(1);
    if(value <= SCALE_EMPTY_VALUE) {
        return 0;
    }
    if(value >= SCALE_FULL_VALUE) {
        return 100;
    }
    return static_cast<int>((value - SCALE_EMPTY_VALUE) * 100.0f / (SCALE_FULL_VALUE - SCALE_EMPTY_VALUE));
}