/* 
 * File:   clock.h
 * Author: penny
 *
 * Created on 2022?1?28?, ??3:40
 */

#ifndef CLOCK_H
#define	CLOCK_H

#include <xc.h>

#define i2c_freq  100
#define _XTAL_FREQ    8000000

unsigned int year = 22;   //2022
unsigned int month = 02;  //December
unsigned int date = 01;   //28
unsigned int hour = 22;   
unsigned int min = 21;
unsigned int sec = 01;
void I2C_Init(const unsigned long freq);
void setTime();
void updateTime();
#endif	/* CLOCK_H */

