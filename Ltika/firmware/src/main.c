/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

#define SYSCLK_DIV1MHZ  48

void delay(unsigned int ms){
    unsigned int i,time;
    time=250*SYSCLK_DIV1MHZ*ms;
    for(i=0;i<time;i++){
        Nop();
    }
}

volatile int Ltika_counter = 0;

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        LED_Set();
        delay(500);
        LED_Clear();
        delay(500);
        Ltika_counter++;
        if(Ltika_counter == 10){
            delay(5000);
        }
        
        Nop();
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

