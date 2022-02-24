/* 
 * File:   irf.h
 * Author: penny
 *
 * Created on 2022?1?29?, ??10:21
 */

#ifndef IRF_H
#define	IRF_H
#include <xc.h>
     
//?????????
#define uchar unsigned char
#define uint unsigned int

////// ///////////////////////////////////////////////////////////////////////////
unsigned char tmr_times;
unsigned char code_data;
unsigned int code;
unsigned char int_times;
unsigned char buf1;
unsigned int  fengcon;
unsigned char head_ok;  	
void init_io(void);
void wrong(int errorid);
unsigned char one_zero(void);

#endif	/* IRF_H */

