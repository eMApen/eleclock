/*
 * File:   eleclockmain.c
 * Author: penny
 *
 * Created on 2022?1?27?, ??11:18
 */


// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define SYS_FREQ        8000000L
#define FCY             SYS_FREQ/4
#define i2c_freq  100
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "ht1621.h"
#include "clock.h"
#include "irf.h"

#define Year 0x1
#define Date 0x2
#define Time0 0x3
#define Time1 0x4


#define LED RB5
#define LED_Direction TRISB5 
#define LED_ENABLE LED_Direction=0
#define LED_SetBits LED=1
#define LED_ResetBits LED=0
#define DAT 1
#define COMMAND 0



unsigned char Mode,Setting,cursor;
unsigned int IRKey=0;
unsigned char display[4]={0,0,0,0};

void main(void) {
    LED_ENABLE;
    LED_ResetBits;
    HT1621_Init();
    I2C_Init(i2c_freq); 
    init_io();
    Mode = Time0;
    
    updateTime();
//    setTime();
//    IR_data_display(0x2ab9);
    while(1){
        
//     Display Mode - Finite State Machine
        
        if(!Setting){
            updateTime();
            switch(Mode){
                
                case Year:
                    HT1621_Display_Year(year);
                    break;
                case Date:
                    HT1621_Display_Date(month,date);
                    break;
                case Time0:
                    HT1621_Display_Time(0,hour,min);
                    break;
                case Time1:
                    HT1621_Display_Time(1,min,sec);
                    break;
                default:
                    IR_data_display(IRKey);
                    break;
            }
        }
        
        
        switch(IRKey){
            // 1. Setting Mode: push play exit setting mode     -> exit setting mode
            // 2. Mode: push play to change display mode.
            case 0x15:    //Play
                if(Setting){
                    //Save the Setting Nubmber
                    switch(Mode){
                        case Year:
                            year=display[0]*1000+display[1]*100+display[2]*10+display[3];
                            break;
                        case Date:
                            month=display[0]*10+display[1];
                            date=display[2]*10+display[3];
                            break;
                        case Time0:
                            hour=display[0]*10+display[1];
                            min=display[2]*10+display[3];
                            break;
                        case Time1:
                            min=display[0]*10+display[1];
                            sec=display[2]*10+display[3];
                            break;
                        default:
                            break;
                    }
                    setTime();//send it to rtc clock
                    Setting=0;
                }    
                
                IRKey=0;
                break;
                
            // push menu - turn into setting mode
            case 0x47:    //MENU
                if(Setting==0){
                    //transform time into digital display number -> enter setting mode
                    switch(Mode){
                        case Year:
                            display[0]=year/1000;
                            display[1]=year % 1000 / 100;
                            display[2]=year % 1000 % 100 / 10;
                            display[3]=year % 1000 % 100 % 10;
                            break;
                        case Date:
                            display[0]=month / 10;
                            display[1]=month % 10;
                            display[2]=date / 10;
                            display[3]=date % 10;
                            break;
                        case Time0:
                            display[0]=hour / 10;
                            display[1]=hour % 10;
                            display[2]=min / 10;
                            display[3]=min % 10;
                            break;
                                    
                        case Time1:
                            display[0]=min / 10;
                            display[1]=min % 10;
                            display[2]=sec / 10;
                            display[3]=sec % 10;
                            break;
                        default:
                            break;
                    }
                    Setting=1;
                    cursor=0;
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
            case 0x16:    //0
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=0;break;
                        case 1: display[1]=0;break;
                        case 2: display[2]=0;break;
                        case 3: display[3]=0;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
            case 0x0C:    //1
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=1;break;
                        case 1: display[1]=1;break;
                        case 2: display[2]=1;break;
                        case 3: display[3]=1;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
                
            case 0x18:    //2
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=2;break;
                        case 1: display[1]=2;break;
                        case 2: display[2]=2;break;
                        case 3: display[3]=2;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
                
            case 0x5E:    //3
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=3;break;
                        case 1: display[1]=3;break;
                        case 2: display[2]=3;break;
                        case 3: display[3]=3;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
                
            case 0x08:    //4
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=4;break;
                        case 1: display[1]=4;break;
                        case 2: display[2]=4;break;
                        case 3: display[3]=4;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
            case 0x1C:    //5
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=5;break;
                        case 1: display[1]=5;break;
                        case 2: display[2]=5;break;
                        case 3: display[3]=5;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
            case 0x5a:    //6
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=6;break;
                        case 1: display[1]=6;break;
                        case 2: display[2]=6;break;
                        case 3: display[3]=6;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
                
            case 0x42:    //7
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=7;break;
                        case 1: display[1]=7;break;
                        case 2: display[2]=7;break;
                        case 3: display[3]=7;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
                
            case 0x52:    //8
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=8;break;
                        case 1: display[1]=8;break;
                        case 2: display[2]=8;break;
                        case 3: display[3]=8;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
                
            case 0x4A:    //9
                if(Setting){
                    switch(cursor){
                        case 0: display[0]=9;break;
                        case 1: display[1]=9;break;
                        case 2: display[2]=9;break;
                        case 3: display[3]=9;break;
                        default: break;
                    }
                    Setting_display(cursor,display);
                }
                IRKey=0;
                break;
                
            case 0x07:    //<<
                if(Setting){
                    if(cursor!=0)cursor--;
                    else cursor=3;
                    Setting_display(cursor,display);
                }
                else{
                    if(Mode!=1)Mode--;
                    else Mode=1;
                }
                
                IRKey=0;
                break;
            case 0x09:    //>>
                if(Setting){
                    if(cursor==3) cursor=0;
                    else cursor++;
                    Setting_display(cursor,display);
                }
                else{
                    if(Mode!=4)Mode++;
                    else Mode=4;
                }
                IRKey=0;
                break;
            case 0x43:  //reverse
                Setting=0;
                IRKey=0;
                break;
            default:
                break;
                
                
        }

        // RTC testing program
        
//        HT1621_Display_Year(year);
//        HT1621_Delay_ms(2000);
//        HT1621_Display_Date(month,date);
//        HT1621_Delay_ms(2000);
//        HT1621_Display_Time(0,hour,min);
//        HT1621_Delay_ms(2000);
//        HT1621_Display_Time(1,min,sec);
//        HT1621_Delay_ms(2000);
//        Setting_display(1,display);
//        HT1621_Delay_ms(2000);
//        Setting_display(2,display);
//        HT1621_Delay_ms(2000);
//        Setting_display(3,display);
//        HT1621_Delay_ms(2000);
//        Setting_display(0,display);
        
        
        // IR testing program
//        HT1621_Delay_ms(2000);
//        IR_data_display(IRKey);
        
        //LCD display testing program
//		  HT1621_Test();

    }
    return;
}

void __interrupt() interrupt_service(){
    if(T0IF==1)         
    {
        TMR0=0x13;       
        T0IF=0;            
 
		tmr_times++;       

    }
	if(CCP2IF==1)         
	{
		CCP2IF=0;           
		GIE=0; 				 
  		int_times++;  		 
  		if(head_ok==0)      
  		{

    		if(tmr_times>100&tmr_times<116)		
    		{
      			head_ok=1;  	
      			int_times=0;    
      			tmr_times=0;          
    		}
    		else
    		{
      			wrong(0xE010);        
    		}
  		}
  		else                	                
  		{    
    		code_data=code_data>>1;
			buf1=one_zero();    		
			tmr_times=0;  		 
    		if(buf1==1)		
    		{
      			code_data|=0x80;  
    		}    	
			else if(buf1==0)		
	        {
      			code_data&=0x7f;  	
    		}    
			else            		
            {
				wrong(0xE002);       		
				return;
			}    						
    		if(int_times==8)    
    		{
			
        		if(code_data!=0x00)  
        		{
        			wrong(0xE003);        
					return;  
        		}    		
    		}
    		else if(int_times==16)        
    		{
				
        		if(code_data!=0xFF)  
        		{
            		wrong(0xE004);             
					return;
        		}	        			
    		}
    		else if(int_times==24)   
    		{
      			code=code_data;   		
    		}
    		else if(int_times==32)    
    		{
      			int_times=0;      	 
      			head_ok=0;          	 			  
      			if((code+code_data)==0xff)  			
      			{          			
            		IRKey= code;           	 
            	}
                else{
                    wrong(0x05);
                }
    		}
  		} 
  		GIE=1;   
  	}   	
}