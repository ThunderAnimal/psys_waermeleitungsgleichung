/**
 * Test for functions in core.c
 *
 * We don't need to test anything here really.
 */
#include <assert.h>

/* Include the source here */
#include "core.c"

/******************************************************/

/**
 * TEST PARAMETER EINGABEN
 */
void test__eingabe(){
    printf("TEST: EINGABE\n");
    //VALIDE EINGABE
    assert(valideWaermeLeitfaehigkeit(0) == 1);
    assert(valideWaermeLeitfaehigkeit(1) == 1);
    assert(valideWaermeLeitfaehigkeit(0.5) == 1);

    //INVALIDE EINGABE
    assert(valideWaermeLeitfaehigkeit(-1) == 0);
    assert(valideWaermeLeitfaehigkeit(2) == 0);
}

/**
 * TEST INIT HEAT MAP
 */
void test__setUpRaster(){
    printf("TEST: SET UP RASTER\n");

    int rasterSize1 = 5;
    float ** heatMap1 =  malloc(sizeof(float*) * rasterSize1);
    for(int i = 0; i < rasterSize1; i++){
        heatMap1[i] = malloc(rasterSize1 * sizeof(float));
    }

    setUpRaster(100, 0, rasterSize1, 3, 2, heatMap1);
    for (int x = 0; x < 5 ; ++x) {
        for (int y = 0; y < 5 ; ++y) {
            if(heatMap1[3][2]){
                assert(heatMap1[3][2] == 100);
            }else{
                assert(heatMap1[x][y] == 0);
            }
        }
    }

    int rasterSize2 = 10;
    float ** heatMap2 =  malloc(sizeof(float*) * rasterSize2);
    for(int i = 0; i < rasterSize2; i++){
        heatMap2[i] = malloc(rasterSize2 * sizeof(float));
    }

    setUpRaster(50, 20, rasterSize2, 3, 2, heatMap2);
    for (int x = 0; x < 10 ; ++x) {
        for (int y = 0; y < 10 ; ++y) {
            if(heatMap2[3][2]){
                assert(heatMap2[3][2] == 50);
            }else{
                assert(heatMap2[x][y] == 20);
            }
        }
    }
    free(heatMap1);
    free(heatMap2);
}

/**
 *  TEST INIT HEAT MAP CENTER HEAT POINT
 */
void test__centerHeatMapRaster(){
    printf("TEST: CENTER HEAT POINT IN RASTER\n");

    int rasterSize1 = 5;
    int rasterSize2 = 10;

    int newRasterSize1 = reziseRasterForCenter(rasterSize1);
    int newRasterSize2 = reziseRasterForCenter(rasterSize2);

    //TEST NEW Size Raster
    assert(newRasterSize1 == 5);
    assert(newRasterSize2 == 11);

    float ** heatMap1 =  malloc(sizeof(float*) * newRasterSize1);
    for(int i = 0; i < newRasterSize1; i++){
        heatMap1[i] = malloc(newRasterSize1 * sizeof(float));
    }
    float ** heatMap2 =  malloc(sizeof(float*) * newRasterSize2);
    for(int i = 0; i < newRasterSize2; i++){
        heatMap2[i] = malloc(newRasterSize2 * sizeof(float));
    }


    setUpRaster(100, 0, newRasterSize1, newRasterSize1/2, newRasterSize1/2,heatMap1);
    assert(heatMap1[2][2] == 100); //in der Mitte vom Raster muss der Hitzepunkt sein
    assert(heatMap1[0][0] == 0);

    setUpRaster(100, 0, newRasterSize2, newRasterSize2/2, newRasterSize2/2,heatMap2);
    assert(heatMap2[5][5] == 100);
    assert(heatMap2[0][0] == 0);

    free(heatMap1);
    free(heatMap2);
}

/**
 * TEST CALC NEXT HEAT MAP
 */
void test__calcNextHeatMap(){
    printf("TEST: CALC NEXT HEAT-MAP\n");

    int rasterSize1 = 3;
    int rasterSize2 = 5;
    
    float ** heatMap1 =  malloc(sizeof(float*) * rasterSize1);
    float ** newHeatMap1 =  malloc(sizeof(float*) * rasterSize1);
    float ** resultHeatMap1 =  malloc(sizeof(float*) * rasterSize1);
    float ** heatMap2 =  malloc(sizeof(float*) * rasterSize2);
    float ** newHeatMap2 =  malloc(sizeof(float*) * rasterSize2);
    float ** resultHeatMap2 =  malloc(sizeof(float*) * rasterSize2);

    for(int i = 0; i < rasterSize1; i++){
        heatMap1[i] = malloc(rasterSize1 * sizeof(float));
        newHeatMap1[i] = malloc(rasterSize1 * sizeof(float));
        resultHeatMap1[i] = malloc(rasterSize1 * sizeof(float));
    }
    for(int i = 0; i < rasterSize2; i++){
        heatMap2[i] = malloc(rasterSize2 * sizeof(float));
        newHeatMap2[i] = malloc(rasterSize2 * sizeof(float));
        resultHeatMap2[i] = malloc(rasterSize2 * sizeof(float));
    }

    heatMap1[0][0] = 0;
    heatMap1[1][0] = 0;
    heatMap1[2][0] = 0;
    heatMap1[0][1] = 0;
    heatMap1[1][1] = 100;
    heatMap1[2][1] = 0;
    heatMap1[0][2] = 0;
    heatMap1[1][2] = 0;
    heatMap1[2][2] = 0;

    resultHeatMap1[0][0] = 0;
    resultHeatMap1[1][0] = 0;
    resultHeatMap1[2][0] = 0;
    resultHeatMap1[0][1] = 0;
    resultHeatMap1[1][1] = 100;
    resultHeatMap1[2][1] = 0;
    resultHeatMap1[0][2] = 0;
    resultHeatMap1[1][2] = 0;
    resultHeatMap1[2][2] = 0;

    heatMap2[0][0] = 0;
    heatMap2[1][0] = 0;
    heatMap2[2][0] = 0;
    heatMap2[3][0] = 0;
    heatMap2[4][0] = 0;
    heatMap2[0][1] = 0;
    heatMap2[1][1] = 0;
    heatMap2[2][1] = 50;
    heatMap2[3][1] = 0;
    heatMap2[4][1] = 0;
    heatMap2[0][2] = 0;
    heatMap2[1][2] = 50;
    heatMap2[2][2] = 100;
    heatMap2[3][2] = 50;
    heatMap2[4][2] = 0;
    heatMap2[0][3] = 0;
    heatMap2[1][3] = 0;
    heatMap2[2][3] = 50;
    heatMap2[3][3] = 0;
    heatMap2[4][3] = 0;
    heatMap2[0][4] = 0;
    heatMap2[1][4] = 0;
    heatMap2[2][4] = 0;
    heatMap2[3][4] = 0;
    heatMap2[4][4] = 0;

    resultHeatMap2[0][0] = 0;
    resultHeatMap2[1][0] = 0;
    resultHeatMap2[2][0] = 0;
    resultHeatMap2[3][0] = 0;
    resultHeatMap2[4][0] = 0;
    resultHeatMap2[0][1] = 0;
    resultHeatMap2[1][1] = 50;
    resultHeatMap2[2][1] = 50;
    resultHeatMap2[3][1] = 50;
    resultHeatMap2[4][1] = 0;
    resultHeatMap2[0][2] = 0;
    resultHeatMap2[1][2] = 50;
    resultHeatMap2[2][2] = 100;
    resultHeatMap2[3][2] = 50;
    resultHeatMap2[4][2] = 0;
    resultHeatMap2[0][3] = 0;
    resultHeatMap2[1][3] = 50;
    resultHeatMap2[2][3] = 50;
    resultHeatMap2[3][3] = 50;
    resultHeatMap2[4][3] = 0;
    resultHeatMap2[0][4] = 0;
    resultHeatMap2[1][4] = 0;
    resultHeatMap2[2][4] = 0;
    resultHeatMap2[3][4] = 0;
    resultHeatMap2[4][4] = 0;

    calcNextHeatMap(1,1,rasterSize1,0.5,heatMap1,newHeatMap1);
    for (int x = 0; x < rasterSize1; x++) {
        for (int y = 0; y < rasterSize1; y++) {
            assert(newHeatMap1[x][y] == resultHeatMap1[x][y]);
        }
    }

    calcNextHeatMap(2,2,rasterSize2,1,heatMap2,newHeatMap2);
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            assert(newHeatMap2[x][y] == resultHeatMap2[x][y]);
        }
    }

    free(heatMap1);
    free(heatMap2);
    free(newHeatMap1);
    free(newHeatMap2);
    free(resultHeatMap1);
    free(resultHeatMap2);
}

/**
 * TEST SET NEW HEATMAP
 */
void test__setNewHeatMap(){
    printf("TEST: SET NEW HEATMAP\n");

    unsigned int rasterSize1 = 3;
    
    float** heatMap1 =  malloc(sizeof(float*) * rasterSize1);
    float** newHeatMap1 =  malloc(sizeof(float*) * rasterSize1);
    for(int i = 0; i < rasterSize1; i++){
        heatMap1[i] = malloc(rasterSize1 * sizeof(float));
        newHeatMap1[i] = malloc(rasterSize1 * sizeof(float));
    }


    newHeatMap1[0][0] = 1;
    newHeatMap1[1][0] = 2;
    newHeatMap1[2][0] = 3;
    newHeatMap1[0][1] = 4;
    newHeatMap1[1][1] = 5;
    newHeatMap1[2][1] = 6;
    newHeatMap1[0][2] = 7;
    newHeatMap1[1][2] = 8;
    newHeatMap1[2][2] = 9;

    setNewRaster(rasterSize1,heatMap1,newHeatMap1);
    for (int x = 0; x < rasterSize1; x++) {
        for (int y = 0; y < rasterSize1; y++) {
            assert(heatMap1[x][y] == newHeatMap1[x][y]);
        }
    }

    free(heatMap1);
    free(newHeatMap1);
}

/**
 * Main entry for the test.
 */
int main(int argc, char **argv)
{
    test__eingabe();
    test__setUpRaster();
    test__centerHeatMapRaster();
    test__calcNextHeatMap();
    test__setNewHeatMap();

    return 0;
}
