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
void init_perifericos(void){
	//Configuracion del AD para RA0 ( pag 105)
	BANKSEL ADCON1 ;
	MOVLW B’10000000’ ;//right justify
	MOVWF ADCON1 ;//Vdd and Vss as Vref
	BANKSEL TRISA ;
	BSF TRISA,0 ;//Set RA0 to input
	BANKSEL ANSEL ;
	BSF ANSEL,0 ;//Set RA0 to analog
	BANKSEL ADCON0 ;
	MOVLW B’11000001’ ;//ADC Frc clock,(esto habra que revisarlo tmb)
	
		
	
}
def leer_RA0(){
	MOVWF ADCON0 ;//AN0, On
	CALL SampleTime ;//Acquisiton delay
	BSF ADCON0,GO ;//Start conversion
	BTFSC ADCON0,GO ;//Is conversion done?
	GOTO $-1 ;//No, test again
	BANKSEL ADRESH ;
	MOVF ADRESH,W ;//Read upper 2 bits
	MOVWF RESULTHI ;//store in GPR space
	BANKSEL ADRESL ;
	MOVF ADRESL,W ;//Read lower 8 bits
	MOVWF RESULTLO ;//Store in GPR space
}
void interrupt manage(void) {
	
	 if (INTCONbits.T0IF == 1) {
		CCPR1L++;
		INTCONbits.T0IF = 0;
	 }
   /*
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
    }*/
}
void init_CCP(void)
{	//pag 126 manual
    CCP1CONbits.P1M = 0b01;//funciona P1D y P1A siempre activo
    CCP1CONbits.CCP1M = 0b0101;//captura cada flanco de subida
	CCP2CONbits.CCP2M = 0b1100;
    CCPR1L = 0;
	CCPR2L = 164;
	PR2 = 164;//cuando tmr2 = pr2 ocurre la magica pg131
	
}

void main(void)
{ 
  OSCCON = 0b00001000; // External cristal
  
  init_TMR0();
  init_TMR2();
  init_con();
  init_CCP();
  TRISC= 0;//?
  while(1){
  
  }
}