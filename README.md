# pic32mxstart_code

「お手軽DIPマイコンのPIC32MXを始めよう(仮称)」のサンプルコードです。PIC32MX250F128BとHarmony3を用いています。

**第3章　環境構築**  
Ltika  
Lチカのコードです。正確にはLチカの3回目のコードです。  
  
**第4章　オシロスコープの製作**  
voltage_sender  
電圧値をUARTで送るコードで正確には2回目のほうです。
  
32MXOscillo, 32MXOscillo_overclock  
オシロスコープのコードです。
  
**第5章　ペリフェラル**  
SPItest_MCP4911  
SPI通信対応の10bitDACであるMCP4911と通信するコードです。
  
I2Ctest_AQM1602  
I2C通信タイプのキャラクタLCDであるAQM1602と通信するコードです。
  
**第6章　USB機器の開発**  
この章で扱う内容はMicrochipのUSBサンプルをベースとしています。  

USBhost_MSD, USBhost_MSD2  
USBメモリに対して書き込みを行うコードと読み込みと書き込みをすることでファイルを簡易コピーするコードです。  
  
USBdevice_HID_keyboard, USBdevice_HID_keyboard2(シークレット)  
USBキーボードのコードです。2のほうは同人誌で紹介していませんがこのページを閲覧した方へのボーナスコードで、扱っている内容はほとんど同じですが表示される文字が変更されています。  

USBdevice_CDC_comport  
(UARTはしていませんが)いわゆるUSBシリアル変換のコードです。  