/**
 * @file heat_conduction.c
 * @brief entryPoint to start the algorithm over CLI
 * 
 * In this file the alorithm will be start over CLI
 * 2 Type to add Parameters 
 * -3-4 Parameters: head_conduction_simple
 * -7-8 Parameters: exec_head_conduction
 * der letzte Parmeter ist jeweils der Modus und Optional
 */

#include <stdlib.h>
#include <stdio.h>

#include "core.h"

/**
 * Main
 * @param argc
 * @param argv
 * @return steps
 * 
 * Params (3-4):
 * - diffEndTemperatur [float]
 * - rasterSize [usigned int]
 * - waermeleitfaehigkeit [float]
 * - modus [int] (optional) (0 - nothing, 1 - debug in console. 2 - GUI)
 *
 * Params (7-8):
 * - diffEndTemperatur [float]
 * - rasterSize [usigned int]
 * - waermeleitfaehigkeit [float]
 * - startTemperature [float]
 * - cornerTemeprature [float]
 * - startPointX [usigned int]
 * - startPointY [usigned int]
 * - modus [int] (optional) (0 - nothing, 1 - debug in console. 2 - GUI)
 */
int main(int argc, char **argv)
{
    float diffEndTemperature;
    unsigned int rasterSize;
    float waermeleitfaehigkeit;
    int modus;
    float startTemperature;
    float cornerTemeprature;
    unsigned int startPointX;
    unsigned int startPointY;


    int steps;

    if(argc < 4){
        fprintf(stderr, "Notenough arguments given!\n");
        return EXIT_FAILURE;
    }

    diffEndTemperature = (float) atof(argv[1]);

    rasterSize = (unsigned int) atoi(argv[2]);
    waermeleitfaehigkeit = (float) atof(argv[3]);

    if(argc >= 8){
        startTemperature = (float) atof(argv[4]);
        cornerTemeprature = (float) atof(argv[5]);
        startPointX = (unsigned int) atoi(argv[6]);
        startPointY = (unsigned int) atoi(argv[7]);

        if(argc == 8){
            modus = 0;
        }
        else{
            /**
              * 0 - aus
              * 1 - debug in console
              * 2 - gui
              */
            modus = atoi(argv[8]);
        }
        steps = exec_head_conduction(startTemperature, cornerTemeprature, diffEndTemperature, rasterSize, startPointX, startPointY, waermeleitfaehigkeit, modus);
    }else{
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
        steps = head_conduction_simple(diffEndTemperature, rasterSize, waermeleitfaehigkeit, modus);
    }





    if(modus != 2){
        printf("#############\nENDE: %d\n", steps);
    }else{
        printf("##,%d", steps);
    }
    return EXIT_SUCCESS;
}
