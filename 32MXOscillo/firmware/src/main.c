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

#include "cobs.h"

#define MIN_SAMPLING_PERIOD 95  //48MHz TMRで2us

typedef enum{
    OSCILLO_WAIT,
    OSCILLO_TRIGGER_PRE,
    OSCILLO_TRIGGER,
    OSCILLO_SAVE
} OscilloMode;

typedef enum{
    OSCILLO_RISE,
    OSCILLO_FALL
} OscilloTrig;

typedef struct{
    OscilloMode mode;           //モード
    uint16_t adc_buf[4096];     //波形データ
    uint32_t counter;           //記録点数のカウンタ
    
    OscilloTrig trig;           //トリガーがどちらか
    uint16_t trig_level;        //10bitADCに合わせる
    uint16_t sampling_period;   //TMRに合わせる
} OscilloState;

typedef struct{
    uint8_t data[10];
    uint32_t counter;
} SerialBuf;

OscilloState oscillo_state = {
    .mode = OSCILLO_WAIT,
    .adc_buf = {0},
    .counter = 0,
    .trig = OSCILLO_RISE,
    .trig_level = 0,
    .sampling_period = MIN_SAMPLING_PERIOD
};

SerialBuf serial_buf = {
    .data = {0},
    .counter = 0
};

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    //PWM
    TMR2_Start();
    OCMP1_Enable();
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        //オシロソフトからデータが届いたら
        if(UART1_ReadCountGet()){
            uint8_t recv;
            if(UART1_Read(&recv, 1)){
                serial_buf.data[serial_buf.counter] = recv;
                serial_buf.counter++;

                //データが0ならCOBSデコードを開始できる
                if(recv == 0){
                    uint8_t oscillo_data[10];
                    int size;
                    cobs_decode(oscillo_data, &size, serial_buf.data, serial_buf.counter);
                    Nop();   
                    serial_buf.counter = 0;

                    //データサイズとSTARTデータを通過したらデータを取り出す
                    if(size == 3 && (oscillo_data[0] & 0x80)){
                        //stateの初期化
                        oscillo_state.counter = 0;
                        oscillo_state.mode = OSCILLO_TRIGGER_PRE;

                        //トリガー情報を取り出す
                        if(oscillo_data[0] & 0x40){
                            oscillo_state.trig = OSCILLO_FALL;
                        }else{
                            oscillo_state.trig = OSCILLO_RISE;
                        }

                        //トリガーレベルは0.1V単位なのでそれを10bitADの値に変換
                        oscillo_state.trig_level = (oscillo_data[0] & 0x3f) * 0.1 / 3.3 * 1024;

                        //2バイトのサンプリング情報を結合
                        //10MHz単位のデータを48MHzでうごくTMRに合わせる
                        oscillo_state.sampling_period = (((oscillo_data[1] << 8) & 0xff00) + oscillo_data[2]) * 4.8 - 1;
                        if(oscillo_state.sampling_period < MIN_SAMPLING_PERIOD){
                            oscillo_state.sampling_period = MIN_SAMPLING_PERIOD;
                        }

                        //TMRを起動
                        TMR3_Initialize();
                        TMR3_PeriodSet(oscillo_state.sampling_period);
                        TMR3_Start();
                        LED_Set();
                    }
                }
            }
        }
        
        if(oscillo_state.mode != OSCILLO_WAIT){
            //AD変換結果を変数に一時保存
            while(!ADC_ResultIsReady());
            uint16_t adc_result = ADC_ResultGet(ADC_RESULT_BUFFER_0);
            AD1CON1CLR = _AD1CON1_DONE_MASK;
            
            if(oscillo_state.mode == OSCILLO_TRIGGER_PRE){
                if(oscillo_state.trig == OSCILLO_RISE && adc_result < oscillo_state.trig_level){
                    oscillo_state.mode = OSCILLO_TRIGGER;
                }else if(oscillo_state.trig == OSCILLO_FALL && adc_result > oscillo_state.trig_level){
                    oscillo_state.mode = OSCILLO_TRIGGER;
                }  
            }else if(oscillo_state.mode == OSCILLO_TRIGGER){
                if(oscillo_state.trig == OSCILLO_RISE && adc_result > oscillo_state.trig_level){
                    oscillo_state.mode = OSCILLO_SAVE;
                    oscillo_state.adc_buf[oscillo_state.counter] = adc_result;
                    oscillo_state.counter++;
                }else if(oscillo_state.trig == OSCILLO_FALL && adc_result < oscillo_state.trig_level){
                    oscillo_state.mode = OSCILLO_SAVE;
                    oscillo_state.adc_buf[oscillo_state.counter] = adc_result;
                    oscillo_state.counter++;
                }   
            }else{
                oscillo_state.adc_buf[oscillo_state.counter] = adc_result;
                oscillo_state.counter++;

                //データ取り完了
                if(oscillo_state.counter == 4000){
                    //AD変換を終了する
                    TMR3_Stop();

                    //通信規格を8bitにしたので8bitに情報を落とす
                    uint8_t transmit_buf[4096];
                    for(int i=0;i<4000;i++){
                        transmit_buf[i] = (oscillo_state.adc_buf[i] >> 2) & 0xff;
                    }

                    //データを送信する
                    for(int i=0;i<40;i++){
                        UART1_Write(&transmit_buf[100*i], 100);
                        while(UART1_WriteCountGet());
                    }

                    oscillo_state.mode = OSCILLO_WAIT;
                    LED_Clear();
                }
            }
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

