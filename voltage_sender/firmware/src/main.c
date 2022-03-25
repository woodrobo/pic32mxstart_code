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

void delayMicroseconds(unsigned int us){
    unsigned int i,time;
    time=(SYSCLK_DIV1MHZ*us) >> 2; 
    for(i=0;i<time;i++){
        Nop();
    }
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    uint16_t adc_result;
    uint16_t counter = 0;
    
    TMR3_Start();
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        while(!ADC_ResultIsReady());
        adc_result = ADC_ResultGet(ADC_RESULT_BUFFER_0);
        AD1CON1CLR = _AD1CON1_DONE_MASK;
        
        printf("[COUNT %5u] VOLTAGE REG:%4u  VALUE:%4.2fV\r",counter, adc_result, (float)adc_result * 3.3 / 1024);
        counter++;
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

