#include <xc.h>
#include <stdio.h>
#include <stdarg.h>
// See /opt/microchip/xc8/v{ver}/docs/chips/16f886.html
// for details on #pragma config

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF
int cont=0; int aumentar=1;
void init_TMR0(void)
{
    OPTION_REGbits.T0CS = 0; //MUX --> Freq of clk
    OPTION_REGbits.PSA = 0; //MUX --> preescaler
    OPTION_REGbits.PS = 0b111; //prescaler factor
    TMR0 = 47; //init
    INTCONbits.T0IE = 1; //enable interruptions TMR0 
}
void init_TMR2(void)
{
    T2CONbits.TMR2ON = 1;
    T2CONbits.TOUTPS = 0b0000;
    T2CONbits.T2CKPS = 0b00;
    TMR2=0;
    
}
void init_con(void)
{   //Enable general interruptions
    INTCONbits.GIE=1;   
    INTCONbits.PEIE=1;
    INTCONbits.T0IE=1;
    //PIE1bits.TMR2IE = 1;
    //PIR1bits.TMR2IF = 0;
}
void interrupt manage(void) {
   
    if (INTCONbits.T0IF == 1) {
        if (cont == 3){
            cont = 0;
            TMR0 = 47;
             if(aumentar==1) {
                CCPR1L++;
                CCPR2L--;
            }
            else {
                CCPR1L--;
                CCPR2L++;
            }
            if (CCPR1L== PR2) {
               aumentar = 0;
			}
            else if(CCPR1L==0) {
               aumentar = 1;
			}
        }   
        else 
            cont++;
        INTCONbits.T0IF = 0;    
    }
}
void init_CCP(void)
{
    CCP1CONbits.P1M = 0b00;
    CCP1CONbits.CCP1M = 0b1100;
    CCP2CONbits.CCP2M = 0b1100;
    CCPR1L = 0;
    CCPR2L = 164;
    PR2 = 164;
}

void main(void)
{ 
  OSCCON = 0b00001000; // External cristal
  
  init_TMR0();
  init_TMR2();
  init_con();
  init_CCP();
  TRISC= 0;
  while(1){
  
  }
}