#include <stdlib.h>
#include <stdio.h>

#include "core.h"

int main(int argc, char **argv)
{
    float diffEndTemperature;
    unsigned int rasterSize;
    float waermeleitfaehigkeit;
    int debug;

    if(argc < 4){
        fprintf(stderr, "Notenough arguments given!\n");
        return EXIT_FAILURE;
    }

    diffEndTemperature = (float) atof(argv[1]);
    rasterSize = (unsigned int) atoi(argv[2]);
    waermeleitfaehigkeit = (float) atof(argv[3]);
    if(argc == 4){
        debug = 0;
    }else{
        debug = atoi(argv[4]);
    }
    int steps = head_conduction_simple(diffEndTemperature, rasterSize, waermeleitfaehigkeit, debug);

    printf("#############\nENDE: %d\n", steps);
    return EXIT_SUCCESS;
}
