#include "pixelColor.h"

unsigned char shadesOfGrey(int steps);

unsigned char stepsToRed(int steps) {
    steps = shadesOfGrey(steps);
    return steps; 
}

unsigned char stepsToGreen(int steps) {
    steps = shadesOfGrey(steps);
    return steps; 
}

unsigned char stepsToBlue(int steps) {
    steps = shadesOfGrey(steps);
    return steps; 
}

unsigned char shadesOfGrey (int steps) {
    unsigned char grey;
    grey = steps;
    return grey;
}
