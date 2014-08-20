//
//  CFD.h
//  FTDI_COM
//
//  Created by riccardo aliberti on 07/07/14.
//  Copyright (c) 2014 riccardo aliberti. All rights reserved.
//

#ifndef __FTDI_COM__CFD__
#define __FTDI_COM__CFD__

#include <iostream>
#include <TString.h>
#include "USBCom.h"

class CFD {
    
public:
    CFD (USBCom *USB, unsigned char address[3], int dacs[3], int CBUS);
    ~CFD ();
    
    void SetDACAddress (unsigned int iDAC, unsigned char addr){ if (iDAC<3) fDACAddr[iDAC] = addr; }
    void SetDACNumber (unsigned int iDAC, unsigned int num) { if (iDAC<3 && num<4) fDACNum[iDAC] = num; }
    void SetDACName (unsigned int iDAC, TString name) { if (iDAC<3) fDACName[iDAC] = name; }
    
    void SetCBUS (unsigned int CBUS) { if (CBUS<8) fCBUS = CBUS; }
    void SetUSBPort (USBCom *usb) { fUSB = usb; }
    
    unsigned char GetDACAddress ( int iDAC ){ if (iDAC < 3) return fDACAddr[iDAC]; else return 0x0; }
    int           GetDACNumber ( int iDAC ){ if (iDAC < 3) return fDACNum[iDAC]; else return -1; }
    TString         GetDACName ( int iDAC ){ if (iDAC < 3) return fDACName[iDAC]; else return ""; }
    
    int GetCBUS (){ return fCBUS; }
    USBCom* GetUSB () { return fUSB; }
    
    unsigned int SetDAC1 (unsigned int val){ return SetDAC(0,val); }
    unsigned int GetDAC1 (){ return GetDAC(0); }
    
    unsigned int SetDAC2 (unsigned int val){ return SetDAC(1,val); }
    unsigned int GetDAC2 (){ return GetDAC(1); }
    
    unsigned int SetDAC3 (unsigned int val){ return SetDAC(2,val); }
    unsigned int GetDAC3 (){ return GetDAC(2); }
    
	unsigned int GetDAC(int DAC_N);
    unsigned int SetDAC(int DAC_N, unsigned int val);

private:
    USBCom *fUSB;
    unsigned char fDACAddr[3];
    int fDACNum[3];
    TString fDACName[3];
    int fCBUS;
    
};

#endif /* defined(__FTDI_COM__CFD__) */
