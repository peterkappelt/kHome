/*
 * config.h
 *
 *  Created on: 16.09.2017
 *      Author: peter
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define SENSOR_NAME             "WZ"                    //a human-readable string without special characters for this device
#define SENSOR_TRANSMITPERIOD   20                      //time in seconds between two transmissions
#define SENSOR_BATTMEASURE      30                      //battery will be measured every SENSOR_TRANSMITPERIOD * SENSOR_BATTMEASURE seconds
#define SENSOR_MAXIDLE          600                     //after a duration of SENOSR_MAXIDLE * SENSOR_TRANSMITPERIOD seconds a transmission will be made, even if no values changed



#endif /* CONFIG_H_ */
