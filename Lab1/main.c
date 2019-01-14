//Lab1
//Jan9, 2019
//Robert Mizuhara
//Willy Enrique
//This program either blinks the LEDs on/off or as a continuous 3-bit binary counter. It also sends messages to the terminal using UART.

//Includes
#include <stdio.h>
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "utils.h"
#include "uart_if.h"
#include "pin_mux_config.h"

#define APPLICATION_VERSION     "1.1.1"

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif


//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************


//*****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES                           
//*****************************************************************************
void LEDBinaryCount(int counter);
void LEDUnisonBlink(int counter);
static void BoardInit(void);
//*****************************************************************************
//                      LOCAL FUNCTION DEFINITIONS                         
//*****************************************************************************

//*****************************************************************************
//
//! Configures the pins as GPIOs and peroidically toggles the lines
//!
//! \param None
//! 
//! This function  
//!    1. Configures 3 lines connected to LEDs as GPIO
//!    2. Sets up the GPIO pins as output
//!    3. Periodically toggles each LED one by one by toggling the GPIO line
//!
//! \return None
//
//*****************************************************************************

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}
//****************************************************************************
//
//! Main function
//!
//! \param none
//! 
//! This function  
//!    -Prints the header message
//!    -Houses a simple state machine to control switch logic and calls the proper LED-blinking functions
//! \return None.
//
//****************************************************************************
int
main()
{
    //Initialize variables
    int counter = 0;
    int state = 0;

    //Initialize Board configurations
    BoardInit();

    //Initialize the Terminal.
    InitTerm();

    //Clearing the Terminal.
    ClearTerm();

    //Print initial instruction message
    Report("\t\t****************************************************\n\r");
    Report("\t\t\t        CC3200 GPIO Application        \n\r");
    Report("\t\t****************************************************\n\r");
    Report("\n\n\n\r");
    Report("\t\t****************************************************\n\r");
    Report("\t\t       Push SW3 to start LED binary counting       \n\r");
    Report("\t\t       Push SW2 to blink LEDs on and off       \n\r");
    Report("\t\t****************************************************\n\r");
    Report("\n\n\n\r");

    // Power on the corresponding GPIO port B for 9,10,11.
    // Set up the GPIO lines to mode 0 (GPIO)
    PinMuxConfig();
    
    //3-State Mealy machine implementation for desired switch functionality
    //initial state waiting for user to go to state 1 or 2
    while(state == 0)
    {
        //if sw3 pressed and sw2 not and in state 0, set state to 1
        if(0x20==GPIOPinRead(GPIOA1_BASE, 0x20) && 0x40!=GPIOPinRead(GPIOA2_BASE, 0x40) && state == 0) {
            state = 1;
        }
        //if sw2 pressed and sw3 not and in state 0, set state to 2
        else if(0x40==GPIOPinRead(GPIOA2_BASE, 0x40) && 0x20!=GPIOPinRead(GPIOA1_BASE, 0x20) && state == 0) {
            state = 2;
        }
    }
    //Checks if stuck in state "loop" between states 1 and 2
    while(state == 1 || state == 2) {
        //if sw3 pressed and sw2 not and in state 1, set state to 2, set pin 18 low, and reset counter
        if(0x20==GPIOPinRead(GPIOA1_BASE, 0x20) && 0x40!=GPIOPinRead(GPIOA2_BASE, 0x40) && state == 1) {
            Report("SW3 pressed\n\r");
            state = 2;
            GPIOPinWrite(GPIOA3_BASE, 0x10, 0x0);
            counter = 0;
        }
        //if sw3 pressed and sw2 not and in state 2, set state to 1, set pin 18 high, and reset counter
        else if(0x40==GPIOPinRead(GPIOA2_BASE, 0x40) && 0x20!=GPIOPinRead(GPIOA1_BASE, 0x20) && state == 2) {
            Report("SW2 pressed\n\r");
            state = 1;
            GPIOPinWrite(GPIOA3_BASE, 0x10, 0x10);
            counter = 0;
        }
        //logic for incrementing/resetting counter variable and passing to LEDBinaryCount function
        if(state == 2) {
            LEDBinaryCount(counter);
            if(counter == 2000000) {

                counter = 0;
            }
            else {
                counter++;
            }
        }
        //logic for incrementing/resetting counter variable and passing to LEDUnisonBlink function
        else if(state == 1) {
            LEDUnisonBlink(counter);
            if(counter == 500000) {
                counter = 0;
            }
            else {
                counter++;
            }
        }
    }
    return 0;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
