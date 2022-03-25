#ifndef _AQM1602_H    /* Guard against multiple inclusion */
#define _AQM1602_H

#include "config/default/peripheral/i2c/master/plib_i2c2_master.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define AQM1602_ADRS    0x3E

//データ書き込み
bool AQM1602_write_data(uint8_t data){
    uint8_t data_arr[2];
    data_arr[0] = 0x40;
    data_arr[1] = data;
    
    if(!I2C2_Write(AQM1602_ADRS, data_arr, 2)){
        return false;
    }   
    while(I2C2_IsBusy());
    if(I2C2_ErrorGet() != I2C_ERROR_NONE){
        return false;
    }
    
    CORETIMER_DelayMs(1);
    
    return true;
}

//コマンド書き込み
bool AQM1602_write_command(uint8_t command){
    uint8_t data_arr[2];
    data_arr[0] = 0x00;
    data_arr[1] = command;
    
    if(!I2C2_Write(AQM1602_ADRS, data_arr, 2)){
        return false;
    }   
    while(I2C2_IsBusy());
    if(I2C2_ErrorGet() != I2C_ERROR_NONE){
        return false;
    }
    
    CORETIMER_DelayMs(10);
    
    return true;
}

//初期化
void AQM1602_init(){
    CORETIMER_DelayMs(100);
    AQM1602_write_command(0x38);
    CORETIMER_DelayMs(20);
    AQM1602_write_command(0x39);
    CORETIMER_DelayMs(20);
    AQM1602_write_command(0x14);
    CORETIMER_DelayMs(20);
    AQM1602_write_command(0x73);
    CORETIMER_DelayMs(20);
    AQM1602_write_command(0x56);
    CORETIMER_DelayMs(20);
    AQM1602_write_command(0x6C);
    CORETIMER_DelayMs(250);
    AQM1602_write_command(0x38);
    CORETIMER_DelayMs(20);
    AQM1602_write_command(0x01);
    CORETIMER_DelayMs(20);
    AQM1602_write_command(0x0C);
    CORETIMER_DelayMs(20);
}

//クリアする
void AQM1602_clear(){
    AQM1602_write_command(0x01);
}

//場所を移動
//row=0-1,col=0-15
void AQM1602_setCursor(uint8_t col, uint8_t row){
    if(row > 1 || col > 15){
        return;
    }
    AQM1602_write_command(0x80 + ((row << 6) & 0x40) + col);
}

//文字列を表示
void AQM1602_print(char str[]){
    for(int i=0;str[i]!=0;i++){
        AQM1602_write_data((uint8_t)str[i]);
    }   
}

//新しい文字を作る(文字コードの0-7に割り当てる)
//num:0-7
//data:8バイトを行として並べる0-4bitが対応
void AQM1602_createChar(uint8_t num, uint8_t data[]){
    if(num>7){
        return;
    }
    AQM1602_write_command(0x40 + ((num << 3) & 0x38));
    for(int i=0;i<8;i++){
        AQM1602_write_data(data[i]);
    }
    AQM1602_setCursor(0, 0);
}
    
/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _AQM1602_H */