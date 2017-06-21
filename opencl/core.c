/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __APPLE__
    #include "OpenCL/opencl.h"
#else
    #include "CL/cl.h"
#endif

#include "core.h"


/* ######## HELPER OpenCL ######## */

/**
 * Read the content of the file given by the filename.
 *
 * @param filename the name of the file which should be read.
 *
 * @return the contents or NULL on an error. The returned
 *  value must be freed with free() when no longer in use.
 */
static char *read_file_contents(const char *filename)
{
    long size;
    char *ret = NULL;
    char *contents = NULL;
    FILE *fh;

    if (!(fh = fopen(filename,"r")))
        return NULL;

    fseek(fh,0,SEEK_END);
    size = ftell(fh);
    if (size < 1)
        goto out;
    fseek(fh,0,SEEK_SET);

    if (!(contents = malloc(size+1)))
        goto out;
    if ((fread(contents, 1, size, fh) != size))
        goto out;

    contents[size] = 0;

    ret = contents;
    contents = NULL;
    out:
    fclose(fh);
    free(contents);
    return ret;
}

/* ######## HELPER ######## */

/**
 * Calc Index in Array from x and y
 * @param pointX
 * @param pointY
 * @param rasterSize
 * @return
 */
unsigned int getIndexFromXY(unsigned int pointX, unsigned int pointY, unsigned int rasterSize){
    return pointY * rasterSize + pointX;
}


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
 * @param sizeArray
 * @param startPointX
 * @param startPointY
 * @param raster
 */
void setUpRaster(float startTemperature,
                    float cornerTemperature,
                    unsigned int rasterSize,
                    unsigned int startPointX,
                    unsigned int startPointY,
                 float* raster){

    for (int i = 0; i < rasterSize * rasterSize ; i++) {
        raster[i] = cornerTemperature;
    }
    raster[getIndexFromXY(startPointX,startPointY,rasterSize)] = startTemperature;
}

/**
 * Set up the new Heat Map
 * @param arraySize
 * @param heatMap
 * @param newHeatMap
 */
void setNewRaster(unsigned int arraySize,
                  float *heatMap,
                  float *newHeatMap,
                    cl_program prog,
                    cl_context context,
                    cl_command_queue cq){

    size_t worksteps = arraySize;
    size_t memworksize =  arraySize * sizeof(float);

    cl_int error;
    cl_mem cl_srcmem, cl_destmem;

    cl_kernel k_copy = clCreateKernel(prog, "copyArray", &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create kernel!\n");
        goto out;
    }

    cl_srcmem = clCreateBuffer(context, CL_MEM_READ_ONLY, memworksize, NULL, &error);
    cl_destmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, memworksize, NULL, &error);
    if (!cl_srcmem || !cl_destmem)
    {
        fprintf(stderr,"Couldn't create buffer!\n");
        goto out;
    }

    error = clSetKernelArg(k_copy, 0, sizeof(cl_srcmem), &cl_srcmem);
    error |= clSetKernelArg(k_copy, 1, sizeof(cl_destmem), &cl_destmem);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Failed to set kernel arguments!\n");
        goto out;
    }


    error = clEnqueueWriteBuffer(cq, cl_srcmem, CL_FALSE, 0, memworksize, newHeatMap, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't transfer source to target mem!\n");
        goto out;
    }

    error = clEnqueueNDRangeKernel(cq, k_copy, 1, NULL, &worksteps, NULL, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't enqueue the kernel!\n");
        goto out;
    }

    error = clEnqueueReadBuffer(cq, cl_destmem, CL_FALSE, 0, memworksize, heatMap, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't read buffer back!\n");
        goto out;
    }

    error = clFinish(cq);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Unable to finish calculation!\n");
        goto out;
    }
    error = CL_SUCCESS;

    out:
    if (cl_srcmem) clReleaseMemObject(cl_srcmem);
    if (cl_destmem) clReleaseMemObject(cl_destmem);
    if (error != CL_SUCCESS){
        fprintf(stderr,"Error number %d\n", error);
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
                     float *heatMap,
                      float *newHeatMap,
                      float *diffTemperature,
                    cl_program prog,
                    cl_context context,
                    cl_command_queue cq){

    unsigned int startIndex = getIndexFromXY(startpointX, startPointY, rasterSize);

    /* Defines the number of work items*/
    size_t worksteps[2];
    worksteps[0] = rasterSize;
    worksteps[1] = rasterSize;
    size_t memworksize =  rasterSize * rasterSize * sizeof(float);

    cl_int error;
    cl_mem cl_srcmem, cl_destmem, cl_destmem2;

    cl_kernel k_calc = clCreateKernel(prog, "calc", &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create kernel!\n");
        goto out;
    }

    cl_srcmem = clCreateBuffer(context, CL_MEM_READ_ONLY, memworksize, NULL, &error);
    cl_destmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, memworksize, NULL, &error);
    cl_destmem2 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, memworksize, NULL, &error);
    if (!cl_srcmem || !cl_destmem || !cl_destmem2)
    {
        fprintf(stderr,"Couldn't create buffer!\n");
        goto out;
    }

    error = clSetKernelArg(k_calc, 0, sizeof(cl_srcmem), &cl_srcmem);
    error |= clSetKernelArg(k_calc, 1, sizeof(cl_destmem), &cl_destmem);
    error |= clSetKernelArg(k_calc, 2, sizeof(cl_destmem2), &cl_destmem2);
    error |= clSetKernelArg(k_calc, 3, sizeof(unsigned int), (void *)&rasterSize);
    error |= clSetKernelArg(k_calc, 4, sizeof(unsigned int), (void *)&startIndex);
    error |= clSetKernelArg(k_calc, 5, sizeof(float), (void *)&waermeleitfaehigkeit);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Failed to set kernel arguments!\n");
        goto out;
    }


    error = clEnqueueWriteBuffer(cq, cl_srcmem, CL_FALSE, 0, memworksize, heatMap, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't transfer source to target mem!\n");
        goto out;
    }

    error = clEnqueueNDRangeKernel(cq, k_calc, 2, NULL, worksteps, NULL, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't enqueue the kernel!\n");
        goto out;
    }

    error = clEnqueueReadBuffer(cq, cl_destmem, CL_FALSE, 0, memworksize, newHeatMap, 0, NULL, NULL);
    error |= clEnqueueReadBuffer(cq, cl_destmem2, CL_FALSE, 0, memworksize, diffTemperature, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't read buffer back!\n");
        goto out;
    }

    error = clFinish(cq);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Unable to finish calculation!\n");
        goto out;
    }
    error = CL_SUCCESS;

    out:
    if (cl_srcmem) clReleaseMemObject(cl_srcmem);
    if (cl_destmem) clReleaseMemObject(cl_destmem);
    if (cl_destmem2) clReleaseMemObject(cl_destmem2);
    if (error != CL_SUCCESS){
        fprintf(stderr,"Error number %d\n", error);
        return 0;
    }
}

float max_diffTemperature(unsigned int arraySize,
                            float *diffTemperature,
                            cl_program prog,
                            cl_context context,
                            cl_command_queue cq){


    //TODO Implements in OpenCl
    unsigned int reduction_array_size = 20;

    float* max = (float*) malloc(reduction_array_size * sizeof(float));

    size_t worksteps = reduction_array_size;
    size_t memworksize =  arraySize * sizeof(float);
    size_t memOutputSze = reduction_array_size * sizeof(float);

    cl_int error;
    cl_mem cl_srcmem, cl_destmem;

    cl_kernel k_max = clCreateKernel(prog, "findMaxValue", &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create kernel!\n");
        goto out;
    }

    cl_srcmem = clCreateBuffer(context, CL_MEM_READ_ONLY, memworksize, NULL, &error);
    cl_destmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, memworksize, NULL, &error);
    if (!cl_srcmem || !cl_destmem)
    {
        fprintf(stderr,"Couldn't create buffer!\n");
        goto out;
    }

    error = clSetKernelArg(k_max, 0, sizeof(cl_srcmem), &cl_srcmem);
    error |= clSetKernelArg(k_max, 1, sizeof(cl_destmem), &cl_destmem);
    error |= clSetKernelArg(k_max, 2, sizeof(int), (void *)&arraySize);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Failed to set kernel arguments!\n");
        goto out;
    }


    error = clEnqueueWriteBuffer(cq, cl_srcmem, CL_FALSE, 0, memworksize, diffTemperature, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't transfer source to target mem!\n");
        goto out;
    }

    error = clEnqueueNDRangeKernel(cq, k_max, 1, NULL, &worksteps, NULL, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't enqueue the kernel!\n");
        goto out;
    }

    error = clEnqueueReadBuffer(cq, cl_destmem, CL_FALSE, 0, memOutputSze, max, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't read buffer back!\n");
        goto out;
    }

    error = clFinish(cq);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Unable to finish calculation!\n");
        goto out;
    }
    error = CL_SUCCESS;

    float maxDiffTemperature = 0.0;
    for(int i = 0; i < reduction_array_size; i++){
        if(max[i] > maxDiffTemperature){
            maxDiffTemperature = max[i];
        }
    }

    out:
    if (cl_srcmem) clReleaseMemObject(cl_srcmem);
    if (cl_destmem) clReleaseMemObject(cl_destmem);
    if (error != CL_SUCCESS){
        fprintf(stderr,"Error number %d\n", error);
        return 0;
    }

    return maxDiffTemperature;

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
 * Output Heat Map
 * @param rasterSize
 * @param raster
 */
void outputHeatMap(int rasterSize, float *raster){
    for(int i = 0; i < rasterSize * rasterSize; i++){
        printf("%f\t", raster[i]);

        if((i + 1) % rasterSize == 0 ){ //Zeilenumbruch
            printf("\n");
        }
    }
}

void outputHeadMapGui(int arraySize, float *raster){
    for (int i = 0; i < arraySize ; i++) {
            printf("%f,", raster[i]);
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
    unsigned int size_Array = rasterSize * rasterSize;
    unsigned int mem_size_Array = sizeof(float) * size_Array;
    float* heatMap = (float*) malloc(mem_size_Array);
    float* diffTemperatureResults = (float*) malloc(mem_size_Array);

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

    //--------------------------------------------
    // INIT OPEN CL
    //--------------------------------------------
    cl_int error;
    cl_platform_id platform;
    cl_device_id device;
    cl_uint platforms, devices;

    char name[128];
    char *k_src = NULL; /* kernel source */
    cl_context context = NULL;

    error = clGetPlatformIDs(1, &platform, &platforms);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't get platform ids\n");
        goto out;
    }

    error = clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(name), name, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't get platform name\n");
        goto out;
    }

    error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, &devices);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't get device ids\n");
        goto out;
    }

    cl_context_properties properties[]=
            {
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
                    0
            };
    context = clCreateContext(properties, 1, &device, NULL, NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create the context\n");
        goto out;
    }

    cl_command_queue cq = clCreateCommandQueue(context, device, 0, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create the command queue\n");
        goto out;
    }

    if (!(k_src = read_file_contents("kernel.cl")))
    {
        fprintf(stderr,"Couldn't read the contents of the kernel file!\n");
        goto out;
    }

    const char *srcptr[]={k_src};
    cl_program prog = clCreateProgramWithSource(context, 1, srcptr, NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create the kernel from source\n");
        goto out;
    }

    error = clBuildProgram(prog, 0, NULL, "", NULL, NULL);
    if (error != CL_SUCCESS )
    {
        char build_c[4096];

        fprintf(stderr, "Error on buildProgram\n");
        fprintf(stderr, "\nRequestingInfo\n");
        clGetProgramBuildInfo( prog, device, CL_PROGRAM_BUILD_LOG, sizeof(build_c), build_c, NULL );
        fprintf(stderr, "Build Log for %s_program:\n%s\n", "kernel", build_c );
        goto out;
    }

    //--------------------------------------------
    //Verarbeitung
    //--------------------------------------------
    do{
        steps = steps + 1;

        //Alocate new Array
        float* newHeatMap = (float*) malloc(mem_size_Array);

        //Calc next HeatMap
        calcNextHeatMap(startPointX, startPointY, rasterSize, waermeleitfaehigkeit, heatMap, newHeatMap,diffTemperatureResults,
                                            prog, context, cq);
        //Get Diif Temperature
        diffTemperature = max_diffTemperature(size_Array, diffTemperatureResults, prog, context, cq);
        //Copy Heat Map
        setNewRaster(size_Array,heatMap,newHeatMap, prog, context, cq);

        free(newHeatMap);
        if(modus == 1){
            outputSteps(steps);
            outputHeatMap(rasterSize, heatMap);
        }else if(modus == 2){
            outputHeadMapGui(size_Array, heatMap);
        }
    }while(diffEndTemperatur < diffTemperature);

    //--------------------------------------------
    //Clean UP and RETURN
    //--------------------------------------------
    out:
    if (context) clReleaseContext(context);
    free(k_src);
    free(heatMap);

    if (error != CL_SUCCESS){
        fprintf(stderr,"Error number %d\n", error);
        return EXIT_FAILURE;
    }

    return steps;
}

/**
 * exce simple head conduction
 *  - with center start heat Point
 *  - start Temperature is 100
 *  - corner Temperatur is 0
 *
 * @param startTemperature
 * @param diffEndTemperatur
 * @param rasterSize
 * @param waermeleitfaehigkeit
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





