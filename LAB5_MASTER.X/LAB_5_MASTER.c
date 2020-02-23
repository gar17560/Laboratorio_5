/*
 * File:   LAB_5_MASTER.c
 * Author: ICHELECE
 *
 * Created on 19 de febrero de 2020, 14:24
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
#include "funciones0.h"
#include <stdint.h>
#define _XTAL_FREQ 8000000

void config (void);
uint8_t contador1;
uint8_t valor_adc;
uint8_t contador = 0;
uint16_t *valor1;
short *valor2;
uint16_t temp;



void main(void) {
    config();
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("S1   S2     S3");
    I2C_Master_Init(100000);      //Initialize I2C Master with 100KHz clock
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x31);
        contador1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(50);
        
        ////////////////////////////////////////////////////////////////////////
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        valor_adc = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(50);
        valor1 = mapear(valor_adc, 255, 5);
        
        ////////////////////////////////////////////////////////////////////////
        I2C_Master_Start();
        I2C_Master_Write(0x07);
        temp = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(50);
        

        ////////////////////////////////////////////////////////////////////////
        
      
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("0x");
        Lcd_Write_Char(uint_to_char(contador1)); 
        __delay_ms(50);
        
        Lcd_Set_Cursor(2,6);
        Lcd_Write_Char(uint_to_char(valor1[0]));
        Lcd_Write_String(".");
        Lcd_Write_Char(uint_to_char(valor1[1]));
        Lcd_Write_Char(uint_to_char(valor1[2]));
        Lcd_Write_String("V");


        valor2 = mapear(temp, -38.2, 125);        
        Lcd_Set_Cursor(2,12);
        Lcd_Write_Char(uint_to_char(valor2[0]));
        Lcd_Write_Char(uint_to_char(valor2[1]));
        Lcd_Write_Char(uint_to_char(valor2[2]));        
        Lcd_Write_Char(0xDF);
        Lcd_Write_String("C");
    }
    
    return;
}


void config (void){
    
    PORTB = 0x00;
    ANSELH = 0x00;
    TRISB = 0x00;   //Todo el puerto A como salida
    
    PORTD = 0x00;
    TRISD = 0x00;
    PORTA = 0;
    TRISA = 0;
    ANSEL = 0;
   
}