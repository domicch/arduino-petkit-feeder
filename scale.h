#ifndef SCALE_H
#define SCALE_H

#include "HX711.h"

class Scale {
    private:
        // PINs
        const int PIN_SCALE_SCK = 12;
        const int PIN_SCALE_DATA = 11;

        const float SCALE_EMPTY_VALUE = 1421000.0f;
        const float SCALE_FULL_VALUE = 2077000.0f;

        HX711 scale;

    public:
        void init();

        int getWeightPercentage();
};


#endif