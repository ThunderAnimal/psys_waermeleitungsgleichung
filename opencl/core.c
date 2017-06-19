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
                     float **heatMap,
                      float **newHeatMap){

    float maxDiffTemperatur = 0.0;

    /* Defines the number of work items*/
    size_t worksteps = rasterSize * rasterSize;
    size_t memworksize =  rasterSize * rasterSize * sizeof(float);

    /* The error variable. We need it everywhere */
    cl_int error;

    /* Phase 1 variables */
    cl_platform_id platform;
    cl_device_id device;
    cl_uint platforms, devices;
    char name[128];

    /* Phase 2 variables */
    char *k_src = NULL; /* kernel source */
    cl_context context = NULL;

    /**** Phase 1: Find platform and device (will OpenCL work at all?) *****/

    /* Fetch available platform; we only want the first one. */
    error = clGetPlatformIDs(1, &platform, &platforms);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't get platform ids\n");
        goto out;
    }
    printf("Number of available platforms = %d\n",platforms);

    /* Get the name of the platform and print it */
    error = clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(name), name, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't get platform name\n");
        goto out;
    }
    printf("Platform = %s\n",name);

    /* Now get the first device of our platform */
    error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, &devices);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't get device ids\n");
        goto out;
    }

    /**** Phase 2: Create the context and all of its associates *****/
    /* Create a CL context for the selected device, note that nVidia's OpenCL requires the platform property */
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

    /* Now create the command queue */
    cl_command_queue cq = clCreateCommandQueue(context, device, 0, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create the command queue\n");
        goto out;
    }

    /* Now read in the file with the kernel */
    if (!(k_src = read_file_contents("kernel.cl")))
    {
        fprintf(stderr,"Couldn't read the contents of the kernel file!\n");
        goto out;
    }

    /* Array of all sources, we only have one source file */
    const char *srcptr[]={k_src};

    /* Submit the just loaded source code to OpenCL. The resulting
     * program/kernel is associated with the given context */
    cl_program prog = clCreateProgramWithSource(context, 1, srcptr, NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create the kernel from source\n");
        goto out;
    }

    /* Now compile */
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

    /* Allocate the input memory for the kernel. In the kernel, we only read from it,
     * hence we declare it as read only. */
    cl_mem cl_srcmem1, cl_srcmem2, cl_srcmem3, cl_srcmem4, cl_srcmem5, cl_destmem;
    cl_srcmem1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(unsigned int), NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create read only buffer!\n");
        goto out;
    }
    cl_srcmem2 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(unsigned int), NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create read only buffer!\n");
        goto out;
    }
    cl_srcmem3 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(unsigned int), NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create read only buffer!\n");
        goto out;
    }
    cl_srcmem4 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float), NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create read only buffer!\n");
        goto out;
    }
    cl_srcmem5 = clCreateBuffer(context, CL_MEM_READ_ONLY, memworksize, NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create read only buffer!\n");
        goto out;
    }

    /* In the kernel, we actually copy the contents of the input buffer another buffer.
     * Hence, we declare this buffer as write only.
     */
    cl_destmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, memworksize, NULL, &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create write only buffer!\n");
        goto out;
    }

    /* We now get access to a kernel handle */
    cl_kernel k_example = clCreateKernel(prog, "calc", &error);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't create kernel!\n");
        goto out;
    }
    /* We declare that the cl_srcmem1 is the first parameter */
    error = clSetKernelArg(k_example, 0, sizeof(cl_srcmem1), &cl_srcmem1);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't set first argument for kernel!\n");
        goto out;
    }
    /* We declare that the cl_srcmem2 is the second parameter */
    error = clSetKernelArg(k_example, 1, sizeof(cl_srcmem2), &cl_srcmem2);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't set second argument for kernel!\n");
        goto out;
    }
    /* We declare that the cl_srcmem3 is the second parameter */
    error = clSetKernelArg(k_example, 2, sizeof(cl_srcmem3), &cl_srcmem3);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't set third argument for kernel!\n");
        goto out;
    }
    /* We declare that the cl_srcmem4 is the second parameter */
    error = clSetKernelArg(k_example, 3, sizeof(cl_srcmem4), &cl_srcmem4);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't set fourth argument for kernel!\n");
        goto out;
    }
    /* We declare that the cl_srcmem4 is the second parameter */
    error = clSetKernelArg(k_example, 4, sizeof(cl_srcmem5), &cl_srcmem5);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't set fives argument for kernel!\n");
        goto out;
    }
    /* And we declare that cl_destmem is the third parameter */
    error = clSetKernelArg(k_example, 5, sizeof(cl_destmem), &cl_destmem);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't set sixed argument for kernel!\n");
        goto out;
    }
    /**** Phase 3: Fill and run the command queue *****/
    /* Send input data to OpenCL (async, don't alter the buffer!) */
    error = clEnqueueWriteBuffer(cq, cl_srcmem1, CL_FALSE, 0, sizeof(unsigned int), rasterSize, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't transfer source to target mem!\n");
        goto out;
    }
    error = clEnqueueWriteBuffer(cq, cl_srcmem2, CL_FALSE, 0, sizeof(unsigned int), startpointX, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't transfer source to target mem!\n");
        goto out;
    }
    error = clEnqueueWriteBuffer(cq, cl_srcmem3, CL_FALSE, 0, sizeof(unsigned int), startPointY, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't transfer source to target mem!\n");
        goto out;
    }
    error = clEnqueueWriteBuffer(cq, cl_srcmem4, CL_FALSE, 0, sizeof(float), &waermeleitfaehigkeit, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't transfer source to target mem!\n");
        goto out;
    }
    error = clEnqueueWriteBuffer(cq, cl_srcmem5, CL_FALSE, 0, memworksize, heatMap, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't transfer source to target mem!\n");
        goto out;
    }
    /* Start the "calculation"*/
    error = clEnqueueNDRangeKernel(cq, k_example, 2, NULL, &worksteps, NULL, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't enqueue the kernel!\n");
        goto out;
    }

    /* Read the result back into dest */
    error = clEnqueueReadBuffer(cq, cl_destmem, CL_FALSE, 0, memworksize, newHeatMap, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Couldn't read buffer back!\n");
        goto out;
    }
    /* We have now submitted all commands into the queue. As this was done asynch, we have to
     * wait for completion of all the commands */
    error = clFinish(cq);
    if (error != CL_SUCCESS)
    {
        fprintf(stderr,"Unable to finish calculation!\n");
        goto out;
    }
    error = CL_SUCCESS;

    for(int x = 0; x < rasterSize; x++){
        for (int y = 0; y < rasterSize; y++) {
            printf("%f\n", newHeatMap[x][y]);
        }

    }

    //Calc max diff Temeperatur -->  TODO in kernel mit auslagern
    for (int x = 0; x < rasterSize ; x++) {
        for (int y = 0; y < rasterSize; y++) {
            if(newHeatMap[x][y] - heatMap[x][y] > maxDiffTemperatur){
                maxDiffTemperatur = newHeatMap[x][y] - heatMap[x][y];
            }
        }
    }

    out:
    if (error != CL_SUCCESS)
        fprintf(stderr,"Error number %d\n", error);

    /**** Phase 4: Clean up  *****/
    if (cl_srcmem1) clReleaseMemObject(cl_srcmem1);
    if (cl_srcmem2) clReleaseMemObject(cl_srcmem2);
    if (cl_srcmem3) clReleaseMemObject(cl_srcmem4);
    if (cl_srcmem4) clReleaseMemObject(cl_srcmem4);
    if (cl_srcmem5) clReleaseMemObject(cl_srcmem5);
    if (cl_destmem) clReleaseMemObject(cl_destmem);
    if (context) clReleaseContext(context);

    free(k_src);

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
 * Output Heat Map
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
    for(int i = 0; i < rasterSize; i++){
        heatMap[i] = malloc(rasterSize * sizeof(float));
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
        //Alocate new Array
        float** newHeatMap = malloc(sizeof(float*) * rasterSize);
        for(int i = 0; i < rasterSize; i++){
            newHeatMap[i] = malloc(rasterSize * sizeof(float));
        }

        diffTemperature = calcNextHeatMap(startPointX, startPointY, rasterSize, waermeleitfaehigkeit, heatMap, newHeatMap);
        steps = steps + 1;

        //Copy Heat Map
        setNewRaster(rasterSize,heatMap,newHeatMap);
        free(newHeatMap);

        if(modus == 1){
            outputSteps(steps);
            outputHeatMap(rasterSize, heatMap);
        }else if(modus == 2){
            outputHeadMapGui(rasterSize, heatMap);
        }
    }while(diffEndTemperatur < diffTemperature);

    free(heatMap);
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





