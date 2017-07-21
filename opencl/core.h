/**
 * @file core.h
 * @brief Interface for algorithm
 */

#ifndef _CORE_H
#define _CORE_H

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
 * @param modus (0 - Nothing, 1 - Debug, 2 - GUI)
 * @return count Steps
 */
int exec_head_conduction(float startTemperature,
                         float cornerTemperatur,
                         float diffEndTemperatur,
                         unsigned int rasterSize,
                         unsigned int startPointX,
                         unsigned int startPointY,
                         float waermeleitfaehigkeit,
                         int modus);


/**
 * exce simple head conduction
 *  - with center start heat Point
 *  - start Temperature is 100
 *  - corner Temperature is 0
 *
 * @param diffEndTemperatur
 * @param rasterSize
 * @param waermeleitfaehigkeit
 * @param modus (0 - Nothing, 1 - Debug, 2 - GUI)
 * @return count Steps
 */
int head_conduction_simple(float diffEndTemperatur,
                           unsigned int rasterSize,
                           float waermeleitfaehigkeit,
                           int modus);

#endif
