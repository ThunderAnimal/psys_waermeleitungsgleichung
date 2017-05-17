#ifndef _CORE_H
#define _CORE_H

/**
 * exce the head conduction
 */
int exec_head_conduction(float startTemperature,
                         float cornerTemperatur,
                         float diffEndTemperatur,
                         unsigned int rasterSize,
                         unsigned int startPointX,
                         unsigned int startPointY,
                         float waermeleitfaehigkeit);


/**
 * exce simple head conduction
 *  - with center start heat Point
 *  - corner Temperatur is 0
 */
int head_conduction_simple(float startTemperature,
                           float diffEndTemperatur,
                           unsigned int rasterSize,
                           float waermeleitfaehigkeit);

#endif
