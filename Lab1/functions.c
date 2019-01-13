/*
 * functions.c
 *
 *  Created on: Jan 9, 2019
 *      Author: Robert
 */
#include "hw_types.h"
#include "hw_memmap.h"
#include "gpio.h"
#include "utils.h"

//function that continuously counts from 0 to 7 in binary using the LED on our boards
void LEDBinaryCount(int counter)
{
    if(counter == 0) {
        //000
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x0);
        GPIOPinWrite(GPIOA1_BASE, 0x4, 0x0);
        GPIOPinWrite(GPIOA1_BASE, 0x8, 0x0);
    }
    if(counter == 250000) {
        //001
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);
    }
    if(counter == 500000) {
        //010
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x0);
        GPIOPinWrite(GPIOA1_BASE, 0x4, 0x4);
    }
    if(counter == 750000) {
        //011
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);
    }
    if(counter == 1000000) {
        //100
        GPIOPinWrite(GPIOA1_BASE, 0x8, 0x8);
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x0);
        GPIOPinWrite(GPIOA1_BASE, 0x4, 0x0);
    }
    if(counter == 1250000) {
        //101
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);
    }
    if(counter == 1500000){
        //110
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x0);
        GPIOPinWrite(GPIOA1_BASE, 0x4, 0x4);
    }
    if(counter == 1750000) {
        //111
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);
    }

}

//function that blinks all LEDs on and off repeatedly
void LEDUnisonBlink(int counter){
    if(counter == 0) {
        //on
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x2);
        GPIOPinWrite(GPIOA1_BASE, 0x4, 0x4);
        GPIOPinWrite(GPIOA1_BASE, 0x8, 0x8);
    }
    if(counter == 250000) {
        //off
        GPIOPinWrite(GPIOA1_BASE, 0x2, 0x0);
        GPIOPinWrite(GPIOA1_BASE, 0x4, 0x0);
        GPIOPinWrite(GPIOA1_BASE, 0x8, 0x0);
    }
}

