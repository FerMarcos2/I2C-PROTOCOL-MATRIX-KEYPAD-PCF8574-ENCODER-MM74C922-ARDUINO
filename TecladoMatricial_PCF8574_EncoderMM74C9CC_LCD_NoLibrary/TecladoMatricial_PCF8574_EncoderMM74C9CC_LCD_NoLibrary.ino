/*
              PCF8574 Remote 8-Bit I/O Expander for I2C Bus                                                                 
MIT License                                                                                                                 
                                                                                                                            
Copyright (c) 2020 Fernando Marcos Marcos All right reserved.                                                               
                                                                                                                            
Permission is hereby granted, free of charge, to any person obtaining a copy                                                
of this software and associated documentation files (the "Software"), to deal                                               
in the Software without restriction, including without limitation the rights                                                
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                                                   
copies of the Software, and to permit persons to whom the Software is                                                       
furnished to do so, subject to the following conditions:                                                                    
                                                                                                                            
The above copyright notice and this permission notice shall be included in all                                              
copies or substantial portions of the Software.                                                                             
                                                                                                                            
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                                                  
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                                                    
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                                                 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                                                      
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                                               
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE                                               
SOFTWARE.                                                                                                                   
__________________________________________________________________________________________________________
WEBSITE WITH PRACTICES OF ARDUINO, FPGA, MATLAB AND TOPICS ON DIGITAL ELECTRONICS, ANALOGIC, DIGITAL
SIGNAL PROCESSING, COMMUNICATIONS, ETC.
  www.slideshare.net/FerMarcos2

PROFESSIONAL CURRICULUM
  www.linkedin.com/in/FerMarcos2

YOUTUBE CHANNEL
  www.youtube.com/channel/UCLGgLo5SKhDz3cae4YOQosg

WHATCH THE PROYECT ON YOUTUBE 
  https://youtu.be/wZ7pvYW7ak4
__________________________________________________________________________________________________________
DIGITAL PINS WITH INTERRUPTS
  BOARD                                     DIGITAL PINS USABLE FOR INTERRUPTS
  Uno, Nano, Mini, Other 328-based----------2,3
  Uno Wifi Rev.2, Nano Every----------------all digital pins
  Mega, Mega2560, MegaADK-------------------2,3,18,19,20,21
  Micro, Leonardo, Other 32u4-based---------0,1,2,3,7
  Zero--------------------------------------all digital pins, except 4
  MKR Family boards-------------------------0,1,4,5,6,7,8,9,A1,A2
  Nano 33 IoT-------------------------------2,3,9,10,11,13,15,A5,A7
  Nano 33 BLE, Nano 33 BLE Sense------------all pins
  Due---------------------------------------all digital pins
  101---------------------------------------all digital pins (Only 2,5,7,8,10,11,12,13 work with CHANGE)
__________________________________________________________________________________________________________
ADDRESS PCF8574
  A2-A1-A0    Hex(7bites)
  0--0--0-----0X20
  0--0--1-----0x21
  0--1--0-----0x22
  0--1--1-----0x23
  1--0--0-----0x24
  1--0--1-----0x25
  1--1--0-----0x26
  1--1--1-----0x27

  FOR MORE INFORMATION READ THE DATA SHEET OF EXPANDER
  DATA SHEET PCF8574 http://www.ti.com/lit/ds/symlink/pcf8574.pdf
__________________________________________________________________________________________________________
BUS I2C ARDUINO
  MODELO              SDA   SCK
  Uno, Ethernet       A4    A5
  Nano                A4    A5
  Mini Pro            A4    A5
  Mega2560            20    21
  Due                 20    21--- SDAI, SCLI
  Leonardo            2     3

  FOR MORE INFORMATION
  https://www.arduino.cc/en/Reference/Wire
__________________________________________________________________________________________________________
CONECTION
 MATRIX KEYPAD(4X4)          MM74C922                                  PCF8574
                            ---------- (DATA OUT)       (P-PORT) ----------------- (ADDRESS)
   --------------------X4--|7       17|-----A----~~~~~~----P0---|4               1|----A0---~~-GND 
  |   -----------------X3--|8       16|-----B----~~~~~~----P1---|5               2|----A1---~~-GND
  |   |   -------------X2--|10      15|-----C----~~~~~~----P2---|6               3|----A2---~~-GND
  |   |   |   ---------X1--|11      14|-----D----~~~~~~----P3---|7                |
  |   |   |   |            |          |              GND-~-P4---|9                |
  --------------           |          |              GND-~-P5---|10               |
 |              |          |          |              GND-~-P6---|11               |  ___
 |  7  8  9  %  |------Y1--|1         |              GND-~-P7---|12   14   15   13|--INT---~~-NOT CONNECTED
 |  4  5  6  X  |------Y2--|2         |  __                      ----------------- 
 |  1  2  3  -  |------Y3--|3       13|--OE-~-GND                      |     |
 |  C  0  =  +  |------Y4--|4   12    |                                |     |
 |              |           -----------                               SCL   SDA(BUS I2C PCF8574)
  --------------                 |                       Vcc---R10K----|     |----R10K----Vcc
                           DATA AVAILABLE                              |     |
                                 |                                     |     |
                          PIN WITH INTERRUPT                          SCK   SDA(BUS I2C ARDUINO)
                                 |                                     |     |
                          -----------------------------------------------------------
                         |       2                                    A5     A4      |
                         |                                                           |
                         |                          ARDUINO                          |
                         |                                                           |
                         |                                                           |
                          -----------------------------------------------------------   
__________________________________________________________________________________________________________
*/   

//PROYECTO ON YOUTUBE https://youtu.be/wZ7pvYW7ak4

#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x21, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
volatile byte key=false;
char keyboard[]={'%','9','8','7','x','6','5','4','-','3','2','1','+','=','0','C'};
void setup() {
  Wire.begin(); 
  lcd.begin(16,2);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2),readKey,FALLING); // Or RISING
}
void loop() {
  while(key==true){
    Wire.requestFrom(0x20,1); //Reques Byte from slave device (Address,quantity)
    char a=Wire.read();
    Serial.println(keyboard[Wire.read()]);
    lcd.setCursor(0,0);
    lcd.print(keyboard[a]); 
    key=false;
  }
}
void readKey(){ //Data available
  key = !key; //We press a key so key=true
}
