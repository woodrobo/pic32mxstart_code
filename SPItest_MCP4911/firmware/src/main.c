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

//num:0-1023
void dac_output(uint16_t num){
    uint8_t send_data[2];
    send_data[0] = 0x30 | ((num >> 6) & 0x0f);
    send_data[1] = ((num << 2) & 0xfc);
    
    GPIO_SS_Clear();
    CORETIMER_DelayUs(1);
    SPI2_Write(send_data, 2);
    CORETIMER_DelayUs(1);
    GPIO_SS_Set();
}

uint16_t vol = 0;

void tmr_callback(uint32_t status, uintptr_t context){
    dac_output(vol);
    if(vol < 1023){
        vol++;
    }else{
        vol = 0;
    }
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    TMR2_CallbackRegister(tmr_callback, (uintptr_t)NULL);
    TMR2_Start();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

