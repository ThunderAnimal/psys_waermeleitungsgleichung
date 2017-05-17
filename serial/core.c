/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */
#include <stdio.h>
#include <stdlib.h>

#include "core.h"

/**
 * Rezise Rastersize for Center HEat Point
 * @param rasterSize
 * @return new rasterSize
 */
unsigned int reziseRasterForCenter(unsigned int rasterSize){
    unsigned int centerRasterSize = 0;

    if(rasterSize%2==0){ //Raster Anzahl muss ungerde sein damit der heat Point genau mittig ist
        centerRasterSize = rasterSize + 1;
    }else{
        centerRasterSize =rasterSize;
    }

    return centerRasterSize;
}

/**
 * set Up Start Heat Map
 *
 * @param startTemperature
 * @param cornerTemperature
 * @param rasterSize
 * @param startPointX
 * @param startPointY
 * @param raster
 */
void setUpRaster(float startTemperature,
                    float cornerTemperature,
                    unsigned int rasterSize,
                    unsigned int startPointX,
                    unsigned int startPointY,
                 float raster[rasterSize][rasterSize]){

    for (int i = 0; i < rasterSize ; ++i) {
        for (int k = 0; k < rasterSize ; ++k) {
            raster[i][k] = cornerTemperature;
        }
    }
    raster[startPointX][startPointY] = startTemperature;
}

/**
 * Set up the new Heat Map
 * @param rasterSize
 * @param heatMap
 * @param newHeatMap
 */
void setNewRaster(unsigned int rasterSize,
                  float heatMap [rasterSize][rasterSize],
                  float newHeatMap[rasterSize][rasterSize]){
    for (int x = 0; x < rasterSize ; ++x) {
        for (int y = 0; y < rasterSize ; ++y) {
            heatMap[x][y] = newHeatMap[x][y];
        }
    }
}

/**
 * validate waermeleiffaehigkeit
 * @param waermeleitfaehigkeit
 * @return 0-false 1-true
 */
int valideWaermeLeitfaehigkeit(float waermeleitfaehigkeit){
    if(waermeleitfaehigkeit > 0 && waermeleitfaehigkeit < 1){
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * Output Start Parameter
 * @param startTemperature
 * @param cornerTemperatur
 * @param diffEndTemperatur
 * @param rasterSize
 * @param startPointX
 * @param startPointY
 * @param waermeleitfaehigkeit
 */
void outputStartParameter(float startTemperature,
                          float cornerTemperatur,
                          float diffEndTemperatur,
                          unsigned int rasterSize,
                          unsigned int startPointX,
                          unsigned int startPointY,
                          float waermeleitfaehigkeit){

    printf("Start-Temperatur: %f\n", startTemperature);
    printf("Corner-Temperatur: %f\n", cornerTemperatur);
    printf("DiffEnd-Temperatur: %f\n", diffEndTemperatur);
    printf("Raster-Size: %u\n", rasterSize);
    printf("Startpint: X: %u Y: %u \n", startPointX, startPointY);
    printf("Waermeleitfaehigkeit: %f\n", waermeleitfaehigkeit);
}

void outputSteps(int steps){
    printf("Step: %d\n", steps);
}

/**
 * Output Heat Map
 * @param rasterSize
 * @param raster
 */
void outputHeatMap(int rasterSize, float raster[rasterSize][rasterSize]){
    for (int y = 0; y < rasterSize ; y++) {
        for (int x = 0; x < rasterSize ;x++) {
            printf("%f\t", raster[x][y]);

            if(x == rasterSize -1){ //Zeilenumbruch
                printf("\n");
            }
        }
    }
}

/**
 * Calc Next Step of Heat Map
 * @param startTemperatur
 * @param rasterSize
 * @param heatMap
 * @param newHeatMap
 * @return max diff Temperatur
 */
float calcNextHeatMap(unsigned int startpointX,
                     unsigned int startPointY,
                     unsigned int rasterSize,
                     float waermeleitfaehigkeit,
                     float heatMap [rasterSize][rasterSize],
                      float newHeatMap [rasterSize][rasterSize]){

    float maxDiffTemperatur = 0.0;

    for (int y = 0; y < rasterSize ; ++y) {
        for (int x = 0; x < rasterSize ; ++x) {
            if(x == 0  || y == 0 || x == rasterSize -1 || y == rasterSize - 1){ //wenn an Corner dann Corner Temperatur setzten
                newHeatMap[x][y] = heatMap[0][0];
            }else if( x == startpointX && y == startPointY){ //startpunkt dann übernehmen
                newHeatMap[x][y] = heatMap[startpointX][startPointY];
            }else{ //sonst berechnung der neuen Temperatur an dieser Stelle
                float oldTemperature = heatMap[x][y];
                float newTemperature = oldTemperature + waermeleitfaehigkeit/2 * (heatMap[x + 1][y] + heatMap[x - 1][y] - (2 * oldTemperature)) + waermeleitfaehigkeit/2*(heatMap[x][y + 1] + heatMap[x][y - 1] - (2 * oldTemperature));

                if (newTemperature > oldTemperature){
                    if(newTemperature - oldTemperature > maxDiffTemperatur){
                        maxDiffTemperatur = newTemperature - oldTemperature;
                    }
                    newHeatMap[x][y] = newTemperature;
                }else{
                    newHeatMap[x][y] = oldTemperature;
                }
            }
        }
    }

    return maxDiffTemperatur;
}

/**
 * exce the head conduction
 *
 * @param startTemperature
 * @param cornerTemperatur
 * @param diffEndTemperatur
 * @param rasterSize
 * @param startPointX
 * @param startPointY
 * @param waermeleitfaehigkeit
 * @return count Steps
 */
int exec_head_conduction(float startTemperature,
                         float cornerTemperatur,
                         float diffEndTemperatur,
                         unsigned int rasterSize,
                         unsigned int startPointX,
                         unsigned int startPointY,
                         float waermeleitfaehigkeit){

    int steps = 0;
    float diffTemperature;
    float heatMap[rasterSize][rasterSize];


    //Check Parameter
    if (valideWaermeLeitfaehigkeit(waermeleitfaehigkeit) != 1){
        fprintf(stderr, "Ungueltige Eingabe: Waermeleifaehigkeit, muss zwischen 0-1 liegen");
        return EXIT_FAILURE;
    }

    //Heat Map initialisieren
    setUpRaster(startTemperature, cornerTemperatur, rasterSize, startPointX, startPointY, heatMap);

    //Output Start
    outputStartParameter(startTemperature, cornerTemperatur, diffEndTemperatur, rasterSize, startPointX, startPointY, waermeleitfaehigkeit);
    outputHeatMap(rasterSize, heatMap);

    //Verarbeitung
    do{
        float newHeatMap[rasterSize][rasterSize];

        diffTemperature = calcNextHeatMap(startPointX, startPointY, rasterSize, waermeleitfaehigkeit, heatMap, newHeatMap);
        steps = steps + 1;

        //Copy Heat Map
        setNewRaster(rasterSize,heatMap,newHeatMap);

        outputSteps(steps);
        outputHeatMap(rasterSize, heatMap);
    }while(diffEndTemperatur < diffTemperature);

    return steps;
}

/**
 * exce simple head conduction
 *  - with center start heat Point
 *  - corner Temperatur is 0
 *
 * @param startTemperature
 * @param diffEndTemperatur
 * @param rasterSize
 * @param waermeleitfaehigkeit
 * @return count Steps
*/
int head_conduction_simple(float startTemperature,
                           float diffEndTemperatur,
                           unsigned int rasterSize,
                           float waermeleitfaehigkeit){

    rasterSize = reziseRasterForCenter(rasterSize);
    return exec_head_conduction(startTemperature, 0, diffEndTemperatur, rasterSize, rasterSize/2, rasterSize/2, waermeleitfaehigkeit);
}




