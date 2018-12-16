#include <xc.h>
#include <stdio.h>
// See /opt/microchip/xc8/v1.10/docs/chips/16f886.html
// for details on #pragma config

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000 // necessary for __delay_us

int resul,cont,continuar;

// el numero de veces que hay 5 ms en 1 sg es 200
void init_uart(void)
{  /* See chapter 12.3 PIC16F886 manual (table 12-5) */

  TXSTAbits.BRGH = 0;
  BAUDCTLbits.BRG16 = 0;

  // SPBRGH:SPBRG = 
  SPBRGH = 0;
  SPBRG = 32;  // 9600 baud rate with 20MHz Clock
  
  TXSTAbits.SYNC = 0; /* Asynchronous */
  TXSTAbits.TX9 = 0; /* TX 8 data bit */
  RCSTAbits.RX9 = 0; /* RX 8 data bit */

  PIE1bits.TXIE = 0; /* Disable TX interrupt */
  PIE1bits.RCIE = 0; /* Disable RX interrupt */

  RCSTAbits.SPEN = 1; /* Serial port enable */

  TXSTAbits.TXEN = 0; /* Reset transmitter */
  TXSTAbits.TXEN = 1; /* Enable transmitter */
  
 }

void init_PORTB()
{
    TRISB=0;
}
void init_PORTA()
{
    TRISA=0;
}
void init_Trat()
{
    ADCON0bits.ADCS = 0b10; //Tabla 9.1, para Fosc/32 a 1.6 us a 20 MHz pg 103 manual 
    ADCON0bits.CHS = 0b000;
    ADCON1bits.ADFM = 0b1; //Justificar a la derecha 9.2.6

    ADCON1bits.VCFG1 = 0b0;
    ADCON1bits.VCFG0 = 0b0;
    ADCON0bits.ADON = 0b1; //ADC modo recepcion de datos
    //__delay_ms(1); //estabilizamos el Vin con el delay
    PIE1bits.ADIE = 1;
}
void init_TMR0()
{
    OPTION_REGbits.T0CS=0;
    OPTION_REGbits.PSA=0;
    OPTION_REGbits.PS=0b111;
    TMR0=0;
    INTCONbits.T0IE=1;
}

void interrupt Trat_int(void)
{
    if(INTCONbits.T0IF){
        TMR0=0;
        cont += 1;
        if(cont == 38){
            cont = 0;
            ADCON0bits.GO_DONE = 0b1; //Inicia conversión pg 103
        }
        INTCONbits.T0IF=0;     //desactivar la señal dispara int  
    }
    
    if(PIR1bits.ADIF){
        resul = (ADRESH << 8);
        resul = resul +ADRESL;
        PORTB = resul;
        continuar = 1;
        PIR1bits.ADIF = 0;
        
    }
    
}
void putch(char data)
{ 
    while(!TXSTAbits.TRMT); //12.1 EUSART
    TXREG = data;
 }

void main(void)
{ 
  //  char y;

  // OSCCON = 0b01110001;  // Change to internal 8MHz Clock
  OSCCON = 0b00001000; // External cristal
  OPTION_REG = 0b10000111;

  init_uart();
  init_PORTA();
  init_PORTB();
  init_TMR0();
  init_Trat();
  
  INTCONbits.GIE=1;
  INTCONbits.PEIE = 1;
  
  cont = 0;
  continuar = 0;
  while(1){
      while(!continuar);
      continuar = 0;
      printf("%d\r\n", resul);
  }

}
