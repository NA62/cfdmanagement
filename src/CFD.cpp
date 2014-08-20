//
//  CFD.cpp
//  FTDI_COM
//
//  Created by riccardo aliberti on 07/07/14.
//  Copyright (c) 2014 riccardo aliberti. All rights reserved.
//

//#include "stdafx.h"
#include "CFD.h"


CFD :: CFD(USBCom *USB, unsigned char address[3], int dacs[3], int CBUS) : fUSB(USB),
fCBUS(CBUS){
    
    for (int i=0; i<3; i++){ fDACAddr[i] = (address[i]&0xFE); fDACNum[i] = dacs[i]; }
    
}

CFD :: ~CFD(){

    
}


unsigned int CFD :: SetDAC(int DAC_N, unsigned int val){
    
    fUSB -> SetCBUS(fCBUS);
    
    unsigned char data[3];
    data[0] = fDACAddr[DAC_N];
    data[1] = 0x30;
    data[2] = (unsigned char) val;
    
    switch (fDACNum[DAC_N]) {
        case 0:
            data[1] |= (0x00);
            break;
        case 1:
            data[1] |= (0x01);
            break;
            
        case 2:
            data[1] |= (0x02);
            break;
            
        case 3:
            data[1] |= (0x03);
            break;
            
        default:
            data[1] |= (0xff);
            break;
    }
    
    fUSB -> WriteData(data, 3);
    
    return GetDAC(DAC_N);
    
    
}

unsigned int CFD :: GetDAC(int DAC_N){

    fUSB -> SetCBUS(fCBUS);
    
	 unsigned char data_out[3];
    data_out[0] = (fDACAddr[DAC_N]);
    data_out[1] = 0x90;
    data_out[2] = (fDACAddr[DAC_N]|0x01);
    
    
    switch (fDACNum[DAC_N]) {
        case 0:
            data_out[1] |= (0x00);
            break;
        case 1:
            data_out[1] |= (0x01);
            break;
            
        case 2:
            data_out[1] |= (0x02);
            break;
            
        case 3:
            data_out[1] |= (0x03);
            break;
            
        default:
            data_out[1] |= (0xff);
            break;
    }
    
    int Ndata = 0;
    unsigned char data_in = 0xff;
    //int TimeOut = 10;
    
    //while ((Ndata < 1 || data_in > 0xff) && TimeOut>0){
    
        Ndata = fUSB -> ReadData(data_out,3,&data_in);
    
      //  TimeOut--;
    
    //}

    return  data_in;

}
