/**
 * This is the simple kernel. For the current work item,
 * which can be addressed using the value returned by
 * get_global_id(0), copy the contents from src to dest.
 */
__kernel void calc(__global float *src, __global float *dest, __global float *destDiffTemperatures,
                   unsigned int rasterSize, unsigned int startPointIndex, float waermeleitfaehigkeit)
{
    int x = get_global_id(0);
    int y = get_global_id(1);

    //OpenCl nimmt als Parameter nur 1D Arrays an, daher umrechung der Indexe
    int index =  y * rasterSize + x;
    int indexXPlus1Y = y * rasterSize + x + 1;
    int indexXMinus1Y = y * rasterSize + x - 1;
    int indexXYPlus1 = ( y + 1 ) * rasterSize + x;
    int indexXYminus1 = ( y - 1 ) * rasterSize + x;


    if(x == 0  || y == 0 || x == rasterSize -1 || y == rasterSize - 1){ //wenn an Corner dann Corner Temperatur setzten
        dest[index] = src[0];
        destDiffTemperatures[index]= 0.0;
    }else if(index == startPointIndex){ //startpunkt dann übernehmen
        dest[index] = src[index];
        destDiffTemperatures[index]= 0.0;
    }else{ //sonst berechnung der neuen Temperatur an dieser Stelle
        float oldTemperature = src[index];
        float newTemperature = oldTemperature + waermeleitfaehigkeit/2 * (src[indexXPlus1Y] + src[indexXMinus1Y] - (2 * oldTemperature)) + waermeleitfaehigkeit/2*(src[indexXYPlus1] + src[indexXYminus1] - (2 * oldTemperature));
        if (newTemperature > oldTemperature){ //FIX Problem damit diagonalen immer erst im 2. Schritt berechnet werden und somit negative Werte endstehen koennen
            dest[index] = newTemperature;
            destDiffTemperatures[index]= newTemperature - oldTemperature;
        }else{
            dest[index] = oldTemperature;
            destDiffTemperatures[index]= 0.0;
        }
    }
}

__kernel void copyArray(__global float* src, __global float* dest){
    int i = get_global_id(0);
    dest[i] = src[i];
}