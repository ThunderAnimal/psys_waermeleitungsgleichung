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

    float raster1[5][5];
    setUpRaster(100, 0, 5, 3, 2, raster1);
    for (int x = 0; x < 5 ; ++x) {
        for (int y = 0; y < 5 ; ++y) {
            if(raster1[3][2]){
                assert(raster1[3][2] == 100);
            }else{
                assert(raster1[x][y] == 0);
            }
        }
    }

    float raster2[10][10];
    setUpRaster(50, 20, 10, 3, 2, raster2);
    for (int x = 0; x < 10 ; ++x) {
        for (int y = 0; y < 10 ; ++y) {
            if(raster2[3][2]){
                assert(raster2[3][2] == 50);
            }else{
                assert(raster2[x][y] == 20);
            }
        }
    }

}

/**
 *  TEST INIT HEAT MAP CENTER HEAT POINT
 */
void test__centerHeatMapRaster(){
    printf("TEST: CENTER HEAT POINT IN RASTER\n");

    //TEST NEW Size Raster
    assert(reziseRasterForCenter(5) == 5);
    assert(reziseRasterForCenter(10) == 11);

    float raster5[reziseRasterForCenter(5)][reziseRasterForCenter(5)];
    setUpRaster(100, 0, reziseRasterForCenter(5), reziseRasterForCenter(5)/2, reziseRasterForCenter(5)/2,raster5);
    assert(raster5[2][2] == 100); //in der Mitte vom Raster muss der Hitzepunkt sein
    assert(raster5[0][0] == 0);

    float raster11[reziseRasterForCenter(10)][reziseRasterForCenter(10)];
    setUpRaster(100, 0, reziseRasterForCenter(10), reziseRasterForCenter(10)/2, reziseRasterForCenter(10)/2,raster11);
    assert(raster11[5][5] == 100);
    assert(raster11[0][0] == 0);
}

/**
 * TEST CALC NEXT HEAT MAP
 */
void test__calcNextHeatMap(){
    printf("TEST: CALC NEXT HEAT-MAP\n");

    float heatMap1[3][3] = {{0,0,0}, {0,100,0}, {0,0,0}};
    float newHeatMap1[3][3];
    float resultHeatMap1[3][3] = {{0,0,0}, {0,100,0}, {0,0,0}};

    calcNextHeatMap(1,1,3,0.5,heatMap1,newHeatMap1);
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            assert(newHeatMap1[x][y] == resultHeatMap1[x][y]);
        }
    }

    float heatMap2[5][5] = {{0,0,0,0,0}, {0,0,50,0,0}, {0,50,100,50,0}, {0,0,50,0,0}, {0,0,0,0,0}};
    float newHeatMap2[5][5];
    float resultHeatMap2[5][5] = {{0,0,0,0,0}, {0,50,50,50,0}, {0,50,100,50,0}, {0,50,50,50,0}, {0,0,0,0,0}};

    calcNextHeatMap(2,2,5,1,heatMap2,newHeatMap2);
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            assert(newHeatMap2[x][y] == resultHeatMap2[x][y]);
        }
    }


}

/**
 * TEST SET NEW HEATMAP
 */
void test__setNewHeatMap(){
    printf("TEST: SET NEW HEATMAP\n");

    float heatMap[5][5] = {{0,0,0,0,0}, {0,0,50,0,0}, {0,50,100,50,0}, {0,0,50,0,0}, {0,0,0,0,0}};
    float newHeatMap[5][5] = {{0,0,0,0,0}, {0,50,50,50,0}, {0,50,100,50,0}, {0,50,50,50,0}, {0,0,0,0,0}};

    setNewRaster(5,heatMap,newHeatMap);
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            assert(heatMap[x][y] == newHeatMap[x][y]);
        }
    }
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
