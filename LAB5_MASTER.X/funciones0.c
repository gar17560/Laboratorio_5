/*
 * File:   funciones0.c
 * Author: MIGUEL GARCÍA
 *
 * Created on 19 de febrero de 2020, 15:13
 */


#include <xc.h>
#include "funciones0.h"
#include <stdint.h>

#define _XTAL_FREQ 4000000 //Frecuencia de trabajo

#define RS PORTDbits.RD6
#define EN PORTDbits.RD7

#define D0 PORTBbits.RB0
#define D1 PORTBbits.RB1
#define D2 PORTBbits.RB2
#define D3 PORTBbits.RB3
#define D4 PORTBbits.RB4
#define D5 PORTBbits.RB5
#define D6 PORTBbits.RB6
#define D7 PORTBbits.RB7

#define DATA PORTB 


void Lcd_Init(void) {
    __delay_ms(20);
    Lcd_Cmd(0x30);
    __delay_ms(5);
    Lcd_Cmd(0x30);
    __delay_us(100);
    Lcd_Cmd(0x30);
    __delay_us(100);////////////////////////////////////
    Lcd_Cmd(0x38);
    __delay_us(53);
    Lcd_Cmd(0x08);
    __delay_us(53);
    Lcd_Cmd(0x01);
    __delay_ms(3);
    Lcd_Cmd(0x06);
    __delay_us(53);
    Lcd_Cmd(0x0C);
    __delay_us(53);
}

void Lcd_Cmd(int a) {
	RS = 0;             // => RS = 0
    __delay_ms(5);
    EN = 1;
    __delay_ms(5);
	DATA = a;
    __delay_ms(5);
    EN  = 0;             // => E = 0
    __delay_ms(5);
}

void Lcd_Clear(void)
{   
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x01);
    __delay_ms(4);
}

void Lcd_Set_Cursor(int a, int b)
{
	int d;
	if(a == 1)
	{
        d = 0x80 + b - 1;
		Lcd_Cmd(d);
	}
	else if(a == 2)
	{
		d = 0xC0 + b - 1;
		Lcd_Cmd(d);
	}
}


void Lcd_Write_Char(char a)
{   
    RS = 1;
    // LCD_Port(caracter);
    DATA = a;
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String(char *a)
{
    int n;
    for(n = 0; a[n] != '\0'; n++){
        Lcd_Write_Char(a[n]);
    }
}

void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.SEN = 1;    //inicia la comunicación i2c
}
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RSEN = 1;   //reinicia la comunicación i2c
}
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Stop()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.PEN = 1;    //detener la comunicación i2c
}
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPBUF = d;             
}
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
short I2C_Master_Read(short a)
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato leído
}
//*****************************************************************************
// Función para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}

char uint_to_char(uint8_t numero){
    char numChr = 214;
    switch (numero){
        case 0:
            numChr = 48;
            break;
        case 1:
            numChr = 49;
            break;
        case 2:
            numChr = 50;
            break;
        case 3:
            numChr = 51;
            break;
        case 4:
            numChr = 52;
            break;
        case 5:
            numChr = 53;
            break;
        case 6:
            numChr = 54;
            break;
        case 7:
            numChr = 55;
            break;
        case 8:
            numChr = 56;
            break;
        case 9:
            numChr = 57;
            break;
        case 10:
            numChr = 65;
            break;
        case 11:
            numChr = 66;
            break;
        case 12:
            numChr = 67;
            break;
        case 13:
            numChr = 68;
            break;
        case 14:
            numChr = 69;
            break;
        case 15:
            numChr = 70;
            break;    
        default:
            numChr = 214;
    }
    return(numChr);
}



uint16_t * uint_to_array(uint8_t numero){
    uint16_t resultado[3] = {0,0,0};
    resultado[0] = numero/100;
    uint8_t centenas = resultado[0];
    resultado[1] = (numero - (centenas *100))/10;
    uint8_t decenas = resultado[1];
    resultado[2] = numero -(centenas*100+decenas*10);
    return(resultado);
}



uint16_t * mapear(uint8_t valor, uint8_t limReal, uint8_t limSup){
    uint16_t resultado[3] = {0,0,0};  // u.d1.d2  [u, d1, d2]
    uint16_t dividendo = valor*limSup;
    while (limReal <= dividendo){
        resultado[0] = resultado[0] + 1;
        dividendo = dividendo - limReal;
    }
    dividendo = dividendo *10;
    while (limReal <= dividendo){
        resultado[1] = resultado[1] +1;
        dividendo = dividendo - limReal;
    }
    dividendo = dividendo *10;
    while (limReal <= dividendo){
        resultado[2] = resultado[2] +1;
        dividendo = dividendo - limReal;
    }
    
    return resultado;
}


