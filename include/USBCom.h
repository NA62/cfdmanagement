//
//  USBCom.h
//  
//
//  Created by riccardo aliberti on 02/07/14.
//
//

#ifndef ____USBCom__
#define ____USBCom__

//#include "stdafx.h"

#include "WinTypes.h"

#include <iostream>
#include <stdio.h>
#include <TString.h>

//#pragma comment(lib, "FTD2XX.lib")
#include "ftd2xx.h"

#define PINS            0x0080 //Set Lower 8 pins of the port (ADBUS 0-7 FT232H)
#define PINS_AC         0x0082 //Set pins of the ACBUS
#define IDLE_PIN_VAL    0x00FF //Set Pins Value in the idle state (0=SCL, 1=SDA_O, 2=SDA_I) FE
#define PIN_DIR         0x00FB //All pins OUT, except for pin 2 IN;
#define PIN_DIR_IN		0x00F9 //All pins OUT, except for pins 2 and 3 IN; (F8)
#define I2CPINS         0x0007 //Pins 0,1,2 ADBUS

#define SET_CLK         0x0086 //Set Clock Divisor
#define CLOCKDIV        0x001B //100 KHz (CB = 29.5 khz, AB = 50 khz, 0B = 500 khz)
#define DIS_CLOCK_DIV_5 0x008A //Disable clock divide-by-5 for 60 MHz Master Clock
#define DIS_AD_CLOCK    0x0097 //Disable adaptive clock
#define TRIFASE_CLOCK   0x008C //Enable trifase clock, data can change on both leading and trailing edge

#define DIS_INT_LOOP    0x0085 //Disable internal loopback
#define EN_DRIVE_0      0x009E //Enable drive 0

#define DATA_IN_B       0x0020 //Clock in data on rising edge
#define DATA_OUT_B      0x0011 //Clock out data on falling edge

#define DATA_IN_b       0x0022 //Clock in 1 bit on rising edge
#define DATA_OUT_b      0x0013 //Clock out 1 bit on falling edge

#define SR_DATA			0x0012 //Clock out 1 bi on rising edge

#define LENGHT_1        0x0000 //Clock in/out 1 bit/byte

#define TIMEOUT 30

#define SEND_BACK       0x0087 //Send back the answer from the i2c dev directly

class USBCom {
    
public:
    USBCom ();
    USBCom (TString SN);
    ~USBCom ();
    
    unsigned char* GetReadData () { return &fDataIn[1]; }
    
    int InitDev (TString SN="");
    bool IsInitialised () { return fInitialised; }
    
    int SendStart ();
    int SendStop ();
	int SendSR ();
    int SendNACK ();

    int WriteData (unsigned char *data, int Ndata);
    int WriteByte (unsigned char data);
    int ReadByte ();
	int ReadData (unsigned char *read_command, int command_length, unsigned char *data);
    
    int SetIDLE ();
    int SetCBUS (int n);
    

private:
    FT_HANDLE fthandle;
    FT_STATUS ftstatus;
    
    unsigned char fDataOut[2048];
    unsigned char fDataIn[2048];
    
    DWORD fNReadByte;
    DWORD fNWriteByte;
    DWORD fNProcessedByte;
    
    bool fInitialised;
    
};


#endif /* defined(____USBCom__) */
