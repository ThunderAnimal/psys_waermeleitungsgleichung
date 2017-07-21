/**
 * @file core.c
 * @brief Implements the core of the algorithm.
 *
 * Contains the hole logic to calc the heatmaps
 */

#include <stdio.h>
#include <stdlib.h>

#include "core.h"



/* ######## HELPER ######## */

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
                 float** raster){

    for (int i = 0; i < rasterSize ; i++) {
        for (int k = 0; k < rasterSize ; k++) {
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
                  float **heatMap,
                  float **newHeatMap){
    for (int x = 0; x < rasterSize ; x++) {
        for (int y = 0; y < rasterSize ; y++) {
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
    if(waermeleitfaehigkeit >= 0 && waermeleitfaehigkeit <= 1){
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * Calc Next Step of Heat Map
 * @param startpointX
 * @param startPointY
 * @param rasterSize
 * @param waermeleitfaehigkeit
 * @param heatMap
 * @param newHeatMap
 * @return max diff Temperatur
 */
float calcNextHeatMap(unsigned int startpointX,
                     unsigned int startPointY,
                     unsigned int rasterSize,
                     float waermeleitfaehigkeit,
                     float **heatMap,
                      float **newHeatMap){

    float maxDiffTemperatur = 0.0;

    for (int x = 0; x < rasterSize ; x++) {
        for (int y = 0; y < rasterSize ; y++) {
            if(x == 0  || y == 0 || x == rasterSize -1 || y == rasterSize - 1){ //wenn an Corner dann Corner Temperatur setzten
                newHeatMap[x][y] = heatMap[0][0];
            }else if( x == startpointX && y == startPointY){ //startpunkt dann übernehmen
                newHeatMap[x][y] = heatMap[startpointX][startPointY];
            }else{ //sonst berechnung der neuen Temperatur an dieser Stelle
                float oldTemperature = heatMap[x][y];
                float newTemperature = oldTemperature + waermeleitfaehigkeit/2 * (heatMap[x + 1][y] + heatMap[x - 1][y] - (2 * oldTemperature)) + waermeleitfaehigkeit/2*(heatMap[x][y + 1] + heatMap[x][y - 1] - (2 * oldTemperature));

                if (newTemperature > oldTemperature){ //FIX Problem damit diagonalen immer erst im 2. Schritt berechnet werden und somit negative Werte endstehen koennen
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
/* ############################################################################## */


/* ######## OUTPUT FUNCTIONS FOR UI ######## */

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

/**
 * Output Steps
 * @param steps
 */
void outputSteps(int steps){
    printf("Step: %d\n", steps);
}

/**
 * Output Heat Map in Console
 * @param rasterSize
 * @param raster
 */
void outputHeatMap(int rasterSize, float **raster){
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
 * Output Heat Map for GUI
 * @param rasterSize
 * @param raster
 */
void outputHeadMapGui(int rasterSize, float **raster){
    for (int y = 0; y < rasterSize ; y++) {
        for (int x = 0; x < rasterSize ;x++) {
            printf("%f,", raster[x][y]);
        }
    }
    printf("#,");
    fflush(stdout);
}
/* ############################################################################## */


/* ######## EXEC Functions CALL FROM OUTSIDE ######## */
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
 * @param modus
 * @return count Steps
 */
int exec_head_conduction(float startTemperature,
                         float cornerTemperatur,
                         float diffEndTemperatur,
                         unsigned int rasterSize,
                         unsigned int startPointX,
                         unsigned int startPointY,
                         float waermeleitfaehigkeit,
                         int modus){

    int steps = 0;
    float diffTemperature;

    //Alocate Array
    float** heatMap = malloc(sizeof(float*) * rasterSize);
    float** newHeatMap = malloc(sizeof(float*) * rasterSize);
    for(int i = 0; i < rasterSize; i++){
        heatMap[i] = malloc(rasterSize * sizeof(float));
        newHeatMap[i] = malloc(rasterSize * sizeof(float));
    }

    //Check Parameter
    if (valideWaermeLeitfaehigkeit(waermeleitfaehigkeit) != 1){
        fprintf(stderr, "Ungueltige Eingabe: Waermeleifaehigkeit muss zwischen 0-1 liegen\n");
        return EXIT_FAILURE;
    }

    //Heat Map initialisieren
    setUpRaster(startTemperature, cornerTemperatur, rasterSize, startPointX, startPointY, heatMap);

    //Output Start
    if(modus != 2){
        outputStartParameter(startTemperature, cornerTemperatur, diffEndTemperatur, rasterSize, startPointX, startPointY, waermeleitfaehigkeit);
    }
    if(modus == 1){
        outputHeatMap(rasterSize, heatMap);
    }

    //Verarbeitung
    do{

        diffTemperature = calcNextHeatMap(startPointX, startPointY, rasterSize, waermeleitfaehigkeit, heatMap, newHeatMap);
        steps = steps + 1;

        //Copy Heat Map
        setNewRaster(rasterSize,heatMap,newHeatMap);
     
        if(modus == 1){
            outputSteps(steps);
            outputHeatMap(rasterSize, heatMap);
        }else if(modus == 2){
            outputHeadMapGui(rasterSize, heatMap);
        }
    }while(diffEndTemperatur < diffTemperature);

    free(heatMap);
    free(newHeatMap);
    return steps;
}

/**
 * exce simple head conduction
 *  - with center start heat Point
 *  - start Temperature is 100
 *  - corner Temperatur is 0
 *
 * @param diffEndTemperatur
 * @param rasterSize
 * @param waermeleitfaehigkeit
 * @param modus
 * @return count Steps
*/
int head_conduction_simple(float diffEndTemperatur,
                           unsigned int rasterSize,
                           float waermeleitfaehigkeit,
                           int modus){

    rasterSize = reziseRasterForCenter(rasterSize);
    return exec_head_conduction(100, 0, diffEndTemperatur, rasterSize, rasterSize/2, rasterSize/2, waermeleitfaehigkeit, modus);
}
/* ############################################################################## */





