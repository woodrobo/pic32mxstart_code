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

#include<stdio.h>
#include "AQM1602.h"

char moji[] = "AQM1602XA-RN-GBW";
uint8_t count = 0;

//3本の横線を表す新しい文字
uint8_t chara0[] = {
    0b00000000,
    0b00011111,
    0b00000000,
    0b00011111,
    0b00000000,
    0b00011111,
    0b00000000,
    0b00000000
};

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    AQM1602_init();
    AQM1602_createChar(0, chara0);
    
    AQM1602_print(moji);
    AQM1602_setCursor(0, 1);
    for(int i=0; i<16; i++){
        AQM1602_write_data(i+0xb1); //0xb1はAQM1602で「ア」を示す
    }
    
    CORETIMER_DelayMs(5000);
    AQM1602_clear();
    AQM1602_setCursor(0, 1);
    AQM1602_write_data('a');
    AQM1602_write_data(0);          //登録した文字
    AQM1602_print("b (mod m)");
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        char str[20];
        sprintf(str, "count:%04u", count);
        AQM1602_setCursor(0, 0);
        AQM1602_print(str);
        CORETIMER_DelayMs(1000);
        
        count++;
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/
