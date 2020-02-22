/*
 * File:   funciones2.c
 * Author: MIGUEL GARCÍA
 *
 * Created on 19 de febrero de 2020, 15:13
 */
#include <xc.h>
#include "funciones2.h"

void I2C_Slave_Init(short address)
{
  SSPSTAT = 0x80;
  SSPADD = address;
  SSPCON = 0x36;
  SSPCON2 = 0x01;
  TRISC3 = 1;   //Setting as input as given in datasheet
  TRISC4 = 1;   //Setting as input as given in datasheet
  GIE = 1;
  PEIE = 1;
  SSPIF = 0;
  SSPIE = 1;
}
