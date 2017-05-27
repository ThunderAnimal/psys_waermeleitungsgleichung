#include <stdlib.h>
#include <stdio.h>

#include "core.h"

int main(int argc, char **argv)
{
    float diffEndTemperature;
    unsigned int rasterSize;
    float waermeleitfaehigkeit;
    int modus;

    if(argc < 4){
        fprintf(stderr, "Notenough arguments given!\n");
        return EXIT_FAILURE;
    }

    diffEndTemperature = (float) atof(argv[1]);
    rasterSize = (unsigned int) atoi(argv[2]);
    waermeleitfaehigkeit = (float) atof(argv[3]);
    if(argc == 4){
        modus = 0;
    }else{
        /**
         * 0 - aus
         * 1 - debug in console
         * 2 - gui
         */
        modus = atoi(argv[4]);
    }
    int steps = head_conduction_simple(diffEndTemperature, rasterSize, waermeleitfaehigkeit, modus);

    if(modus != 2){
        printf("#############\nENDE: %d\n", steps);
    }else{
        printf("##,%d", steps);
    }
    return EXIT_SUCCESS;
}
