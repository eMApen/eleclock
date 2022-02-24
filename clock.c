#include "clock.h"

void I2C_Init(const unsigned long freq) {
  TRISC3 = 1;  
  TRISC4 = 1; 
  
  SSPCON  = 0b00101000;
  SSPCON2 = 0b00000000;
  
  SSPADD = (_XTAL_FREQ/(4*freq*100))-1;
  SSPSTAT = 0b00000000; 
}

void I2C_Hold(){
    while ((SSPCON2 & 0b00011111)||(SSPSTAT & 0b00000100)) ;
}

void I2C_Begin(){
  I2C_Hold();  
  SEN = 1;    
}


void I2C_End(){
  I2C_Hold(); 
  PEN = 1;    
}

void I2C_Write(unsigned data){
  I2C_Hold();
  SSPBUF = data; 
}

int  b2d(int to_convert){
   return (to_convert >> 4) * 10 + (to_convert & 0x0F); 
}

int d2b (int to_convert){
   return ((to_convert / 10) << 4) + (to_convert % 10);
}

unsigned short I2C_Read(unsigned short ack){
  unsigned short incoming;
  I2C_Hold();
  RCEN = 1;
  
  I2C_Hold();
  incoming = SSPBUF; 
  
  I2C_Hold();
  ACKDT = (ack)?0:1; 
  ACKEN = 1; 
  
  return incoming;
}

void setTime(){
   I2C_Begin();       
   I2C_Write(0xD0); 
   I2C_Write(0);  
   I2C_Write(d2b(sec)); 
   I2C_Write(d2b(min)); 
   I2C_Write(d2b(hour)); 
   I2C_Write(1);
   I2C_Write(d2b(date));
   I2C_Write(d2b(month));
   I2C_Write(d2b(year));
   I2C_End();
}

void updateTime(){
    I2C_Begin();       
    I2C_Write(0xD0); 
    I2C_Write(0);    
    I2C_End(); 

    I2C_Begin();
    I2C_Write(0xD1);     
    sec = b2d(I2C_Read(1));    
    min = b2d(I2C_Read(1)); 
    hour = b2d(I2C_Read(1));  
    I2C_Read(1);
    date = b2d(I2C_Read(1));  
    month = b2d(I2C_Read(1));  
    year = b2d(I2C_Read(1));  
    I2C_End(); 
     
    I2C_Begin();
    I2C_Write(0xD1);                   
    I2C_Read(1);    
    I2C_End(); 
}