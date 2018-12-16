#include <xc.h>

// See /opt/microchip/xc8/v1.10/docs/chips/16f886.html
// for details on #pragma config

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000 // necessary for __delay_us

char x,cont;

// el numero de veces que hay 5 ms en 1 sg es 200


void init_PORTB()
{
    TRISB=0;
}
void init_TMR0()
{
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.PSA=0;
    OPTION_REGbits.PS=0b111;
    TMR0=157;
    INTCONbits.T0IE=1;
}

void interrupt TMR0_int(void)
{
    TMR0=157;
    cont += 1;
    if(cont == 200){
        x+=1;
        PORTB = x;
        cont = 0;
    }
    INTCONbits.T0IF=0;     //desactivar la señal dispara int  
    
}
void main(void)
{ 
  //  char y;

  // OSCCON = 0b01110001;  // Change to internal 8MHz Clock
  OSCCON = 0b00001000; // External cristal

  init_PORTB();
  init_TMR0();
  INTCONbits.GIE=1;
  
  cont = 0;
  x = 0;
  
  while(1)
  {
      
   } 
}