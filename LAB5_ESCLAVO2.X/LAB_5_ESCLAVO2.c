/*
 * File:   LAB_5_ESCLAVO2.c
 * Author: MIGUEL GARCÍA
 *
 * Created on 19 de febrero de 2020, 14:52
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include "funciones2.h"

#define _XTAL_FREQ 8000000

uint8_t a;
uint8_t b;
uint8_t contador = 0;
short z;
uint8_t recibir1;


void config (void);
void I2C_Slave_Init(short address);

void __interrupt() isr(void){
    if (INTCONbits.RBIF == 1){
        if (PORTBbits.RB0==0 && a==0){    
            a = 1;
            if(contador >0x00){
                contador--;
            }
        }
        if (PORTBbits.RB1==0 && b ==0){    
            b = 1;
            if(contador < 0x0F){
                contador++;
        }
        }
        INTCONbits.RBIF = 0;   
    }
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            recibir1 = SSPBUF;             // Guardar en recibir1 el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = contador;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
    return;
}

void main(void) {
    config();
    I2C_Slave_Init(0x30);
    while(1){
        PORTA = contador;
        if(PORTBbits.RB0 == 1){
            a=0;
        }
        if(PORTBbits.RB1 == 1){
            b=0;
        }        
        __delay_ms(100);
    }
    
    return;
}


void config (void){
    
    PORTA = 0x00;
    ANSEL = 0x00;
    TRISA = 0x00;   //Todo el puerto A como salida
    
    PORTB = 0x00;
    TRISB = 0b00000011;
    ANSELH = 0x00;
    
    WPUB = 0b11111111;
    OPTION_REGbits.nRBPU = 0;  //Puertp B como entrada con PULL-UP
    IOCB = 0x03;                //Habilitamos las interrupciones para el puerto B en RB0 y RB1
    
    ////////////////////////////////////////////////////////////////////////////
    ////    INTERRUPCIONES PUERTO B
    ////////////////////////////////////////////////////////////////////////////
    ei();
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.PEIE = 1;    
    
}


void I2C_Slave_Init(short address) 
{
  SSPSTAT = 0x80;    
  SSPADD = address; //Setting address
  SSPCON = 0x36;    //As a slave device
  SSPCON2 = 0x01;
  TRISC3 = 1;       //Setting as input as given in datasheet
  TRISC4 = 1;       //Setting as input as given in datasheet
  GIE = 1;          //Global interrupt enable
  PEIE = 1;         //Peripheral interrupt enable
  SSPIF = 0;        //Clear interrupt flag
  SSPIE = 1;        //Synchronous serial port interrupt enable
}