//
//  USBCom.cpp
//
//
//  Created by riccardo aliberti on 02/07/14.
//
//
#define Sleep(int) sleep(int)

//#include "stdafx.h"
#include "USBCom.h"

#include <string>
#include <unistd.h>


using namespace std;

USBCom :: USBCom (){
    
    InitDev();
}

USBCom :: USBCom (TString SN){
    
    InitDev(SN);
}


USBCom :: ~USBCom (){
    
    
}

int USBCom :: InitDev(TString SN){
    
    fInitialised = false;
    bool SearchBySN = true;
    
    if (SN.Capacity()<1) SearchBySN = false;
    
    //Opening the Device
    DWORD Ndev = 0;
    ftstatus = FT_CreateDeviceInfoList(&Ndev);
    
    if (ftstatus!=FT_OK){
        cout <<"Could Not Create the list of Devices"<<endl;
        return 1;
    }
    
    FT_DEVICE_LIST_INFO_NODE *dev_info_list = new FT_DEVICE_LIST_INFO_NODE [Ndev];
    ftstatus = FT_GetDeviceInfoList(dev_info_list, &Ndev);
    
    bool found = false;
    for (unsigned int i=0; i<Ndev; i++){
        string descr = dev_info_list[i].Description;
        cout <<" id = "<<dev_info_list[i].ID<<endl;
        cout <<" Serial Number = ";
        printf("%s",dev_info_list[i].SerialNumber);
        cout <<endl;
        cout <<"descr = ";
        printf("%s",dev_info_list[i].Description);
        cout<<endl;
        
        if (SearchBySN && !SN.EqualTo(dev_info_list[i].SerialNumber)) continue;
        
        
        //fthandle = dev_info_list[i].ftHandle;
        ftstatus = FT_Open(i, &fthandle);
        if (ftstatus != FT_OK) {
            cout <<"couldn't open the device"<<endl;
            cout <<"exit status: "<<ftstatus<<endl;
            continue;
        }
        else found = true;
        
        break;
    }
    if (!found){
        cout <<"device not found"<<endl;
        return 1;
    }
	else cout <<"device opened!"<<endl;
    
    //Resetting, Purging Cache and Setting USB Parameters
    ftstatus = FT_ResetDevice(fthandle);
    if (ftstatus != FT_OK) {
        cout <<"couldn't reset the device"<<endl;
    }
    
    ftstatus = FT_GetQueueStatus(fthandle, &fNReadByte);
	Sleep(5);
    if (ftstatus == FT_OK) FT_Read(fthandle, fDataIn, fNReadByte, &fNProcessedByte);
    Sleep(5);
    
    //ftstatus |= FT_SetUSBParameters(fthandle, 65536, 65536);			// Set USB request transfer sizes
    ftstatus |= FT_SetUSBParameters(fthandle, 10,10);
    ftstatus |= FT_SetChars(fthandle, false, 0, false, 0);				// Disable event and error characters
    ftstatus |= FT_SetTimeouts(fthandle, 50, 50);					// Set the read and write timeouts to 5 seconds
    ftstatus |= FT_SetLatencyTimer(fthandle, 16);						// Keep the latency timer at default of 16ms
    ftstatus |= FT_SetBitMode(fthandle, 0x0, 0x00); 					// Reset the mode to whatever is set in EEPROM
    ftstatus |= FT_SetBitMode(fthandle, 0x0, 0x02);	 					// Enable MPSSE mode
    
    if (ftstatus != FT_OK) {
        cout <<"Failed in device initialization"<<endl;
        FT_Close(fthandle);
        return 1;
    }
    
    cout <<"Device initialized correctly"<<endl;
    //Syncronising MPSSE protocol sending bad commands 0xAA and 0xAB
    
    for (int j=0; j<2; j++){
        fNWriteByte = fNReadByte = 0;
		fNProcessedByte = 0;
        
		switch (j) {
            case 0:
                fDataOut[fNWriteByte++] = 0xAA;
                break;
                
            case 1:
                fDataOut[fNWriteByte++] = 0xAB;
                break;
                
            default:
                break;
        }
        
        
        ftstatus = FT_Write(fthandle, fDataOut, fNWriteByte, &fNProcessedByte);
        
        if (fNWriteByte != fNProcessedByte) {
            printf("Failed in sending sync byte %4x\n",(j==0 ? 0xAA : 0xAB));
            FT_Close(fthandle);
            return 1;
        }
        printf ("Sent sync byte %4x\n",(j==0 ? 0xAA : 0xAB));
        
        fNReadByte = 0;
        ftstatus = FT_OK;
        
        ftstatus = FT_GetQueueStatus(fthandle, &fNReadByte);
        
        int ReadTimeOut = TIMEOUT;
        fNProcessedByte = 0;
        
        while (ReadTimeOut>0 && fNReadByte<2 && ftstatus==FT_OK) {
            ReadTimeOut--;
            ftstatus = FT_GetQueueStatus(fthandle, &fNReadByte);
            Sleep(1);
        }
        
        if (ftstatus!=FT_OK || fNReadByte<2){
            printf("Syncronisation failed ( %4x| TimeOut)\n",(j==0 ? 0xAA : 0xAB));
            FT_Close(fthandle);
            return 1;
        }
        
		printf("First part of the sync is ok ( %4x), found %d bytes to be read\n",(j==0 ? 0xAA : 0xAB), fNReadByte);
        
		ReadTimeOut = TIMEOUT;
        
		while (ReadTimeOut>=0 && fNProcessedByte<fNReadByte && ftstatus==FT_OK) {
            ReadTimeOut--;
            ftstatus = FT_Read(fthandle, fDataIn, fNReadByte, &fNProcessedByte);
			Sleep(1);
        }
        
        if (ftstatus!=FT_OK || fNReadByte != fNProcessedByte){
            
            printf("Syncronisation failed ( %4x| Read)\n",(j==0 ? 0xAA : 0xAB));
            FT_Close(fthandle);
            return 1;
            
        }
        
		printf("Second part of the sync is ok ( %4x)\n read %d byte\n",(j==0 ? 0xAA : 0xAB),fNProcessedByte);
        
        bool ok = false;
        for (unsigned int i=0; i<fNProcessedByte-1; i++){
			if (i > 2048) break;
            if (j==0 && ((fDataIn[i]!=0xFA) || (fDataIn[i+1]!=0xAA))) continue;
            else if (j==1 && ((fDataIn[i]!=0xFA) || (fDataIn[i+1]!=0xAB))) continue;
            ok = true;
            break;
        }
        
        if (!ok){
            printf("Syncronisation failed %4x\n",(j==0 ? 0xAA : 0xAB));
            FT_Close(fthandle);
            return 1;
        }
        
		printf("Syncronization successed ( %4x)\n",(j==0 ? 0xAA : 0xAB));
        
    }
    
    //Configure the MPSSE Settings
    
    fNWriteByte = 0;
    
    fDataOut[fNWriteByte++] = DIS_CLOCK_DIV_5;
    fDataOut[fNWriteByte++] = DIS_AD_CLOCK;
    fDataOut[fNWriteByte++] = TRIFASE_CLOCK;
    fDataOut[fNWriteByte++] = EN_DRIVE_0;
    fDataOut[fNWriteByte++] = I2CPINS; //Enable drive 0 on the I2C pins
    fDataOut[fNWriteByte++] = 0x0;      //Don't enable on AC BUS
    fDataOut[fNWriteByte++] = DIS_INT_LOOP;
    
    ftstatus = FT_Write(fthandle, fDataOut, fNWriteByte, &fNProcessedByte);
    
    fNWriteByte = 0;
    
    fDataOut[fNWriteByte++] = SET_CLK;                  //Set Clock divisor
    fDataOut[fNWriteByte++] = CLOCKDIV & 0xFF;          //Clk = 60 MHz / {(1 + [(1 + CLOCKDIV<<8)|(CLOCKDIV)]*2)}
    fDataOut[fNWriteByte++] = (CLOCKDIV >> 8) & 0xFF;   //
    
    ftstatus = FT_Write(fthandle, fDataOut, fNWriteByte, &fNProcessedByte);
    
    Sleep(1);
    
    fInitialised = true;
    
    return SetIDLE();
}


int USBCom :: SetIDLE (){
    
    
    fNWriteByte = 0;
    
    fDataOut[fNWriteByte++] = PINS;
    fDataOut[fNWriteByte++] = IDLE_PIN_VAL;
    fDataOut[fNWriteByte++] = PIN_DIR;
    
    fDataOut[fNWriteByte++] = PINS_AC;
    fDataOut[fNWriteByte++] = 0x00; //All pins low
    fDataOut[fNWriteByte++] = 0xFF; //All pins output
    
    ftstatus = FT_Write(fthandle, fDataOut, fNWriteByte, &fNProcessedByte);
    
    if (ftstatus==FT_OK) return 0;
    else return 1;
 }

int USBCom :: SendStart(){
    
    fNWriteByte = 0;
    
    for (int i=0; i<10; i++) { //To ensure minimum stay time;
        fDataOut[fNWriteByte++] = PINS;
        fDataOut[fNWriteByte++] = 0xFD; //Put SDA line low (SCL high)
        fDataOut[fNWriteByte++] = PIN_DIR;
    }
    
    for (int i=0; i<5; i++) { //To ensure minimum stay time;
        fDataOut[fNWriteByte++] = PINS;
        fDataOut[fNWriteByte++] = 0xFC; //Put also SCL line low
        fDataOut[fNWriteByte++] = PIN_DIR;
    }

	return fNWriteByte;
    
}

int USBCom :: SendSR(){

	for (int i=0; i<2; i++){
	fDataOut[fNWriteByte++] = PINS;
	fDataOut[fNWriteByte++] = IDLE_PIN_VAL;
	fDataOut[fNWriteByte++] = PIN_DIR;
	}

	for (int i=0; i<2; i++){
	fDataOut[fNWriteByte++] = PINS;
	fDataOut[fNWriteByte++] = 0xFD;
	fDataOut[fNWriteByte++] = PIN_DIR;
	}

	for (int i=0; i<2; i++){
	fDataOut[fNWriteByte++] = PINS;
	fDataOut[fNWriteByte++] = 0xFC;
	fDataOut[fNWriteByte++] = PIN_DIR;
	}
	return fNWriteByte;

}

int USBCom :: SendStop(){
    
	//fNWriteByte = 0;

    for (int i=0; i<10; i++) { //To ensure minimum stay time;
        fDataOut[fNWriteByte++] = PINS;
        fDataOut[fNWriteByte++] = 0xFC; //Put SDA and SCL lines low
        fDataOut[fNWriteByte++] = PIN_DIR;
    }
    
    
    for (int i=0; i<10; i++) { //To ensure minimum stay time;
        fDataOut[fNWriteByte++] = PINS;
        fDataOut[fNWriteByte++] = 0xFD; //Put SCL line high
        fDataOut[fNWriteByte++] = PIN_DIR;
    }
    
    
    for (int i=0; i<4; i++) { //To ensure minimum stay time;
        fDataOut[fNWriteByte++] = PINS;
        fDataOut[fNWriteByte++] = 0xFF; //Put also SDA line high
        fDataOut[fNWriteByte++] = PIN_DIR;
    }

	return fNWriteByte;

}

int USBCom :: SendNACK(){
    
    //fNWriteByte = 0;
    fDataOut[fNWriteByte++] = DATA_OUT_b;
    fDataOut[fNWriteByte++] = LENGHT_1;
    fDataOut[fNWriteByte++] = 0xFF; //byte from which to send the bit;

	return fNWriteByte;
    
}

int USBCom :: ReadByte(){
        
    //Read the Byte
    fDataOut[fNWriteByte++] = DATA_IN_B;
    fDataOut[fNWriteByte++] = LENGHT_1;
    fDataOut[fNWriteByte++] = LENGHT_1;
   
    return fNWriteByte;
}

int USBCom :: WriteByte(unsigned char data){
    
    //fNWriteByte = 0;
    
    //Write a byte
    fDataOut[fNWriteByte++] = DATA_OUT_B;
    fDataOut[fNWriteByte++] = LENGHT_1;
    fDataOut[fNWriteByte++] = LENGHT_1;
    fDataOut[fNWriteByte++] = data;
    
    cout <<"Writing "<<Form("%04x",data)<<endl;
    
    //Set IDLE to read the ACK
    fDataOut[fNWriteByte++] = PINS;
    fDataOut[fNWriteByte++] = 0xF8;
    fDataOut[fNWriteByte++] = PIN_DIR_IN;
    
    //Read ACK
    fDataOut[fNWriteByte++] = DATA_IN_b;
    fDataOut[fNWriteByte++] = LENGHT_1;
    
    //Get the result immediatelly
    //fDataOut[fNWriteByte++] = SEND_BACK;
    
	//Set IDLE to read the ACK
	for (int i=0; i<2; i++){ 
    fDataOut[fNWriteByte++] = PINS;
    fDataOut[fNWriteByte++] = 0xF8;
    fDataOut[fNWriteByte++] = PIN_DIR;
	}
    

   return fNWriteByte;;
    
}

int USBCom :: WriteData(unsigned char *data, int Ndata){
    //ftstatus = FT_Purge(fthandle, FT_PURGE_RX | FT_PURGE_TX);
	fNProcessedByte = 0;
    
	SendStart();
    
    for (int i=0; i<Ndata; i++){
        
        WriteByte(data[i]);
        //if (status!=0) return 1;
        
    }

	SendStop();
    
	ftstatus = FT_Write(fthandle, fDataOut, fNWriteByte, &fNProcessedByte);

	int timeout = TIMEOUT;
	fNReadByte = 0;

	while (timeout >= 0 && fNReadByte < 1 && ftstatus==FT_OK){ 
		ftstatus = FT_GetQueueStatus(fthandle, &fNReadByte);
		timeout--;
		Sleep(1);
	}
    
    if (ftstatus!=FT_OK || fNReadByte<3) cout <<"Not able to read ACKs (0): "<<fNReadByte<<endl;
    
	
			fNProcessedByte = 0;

			while (timeout >= 0 && fNProcessedByte < fNReadByte && ftstatus==FT_OK){ 
			ftstatus = FT_Read(fthandle, fDataIn, fNReadByte, &fNProcessedByte);
			timeout--;
			Sleep(1);
		}

		 if (ftstatus!=FT_OK || fNProcessedByte<3) cout <<"Not able to read ACKs (1): "<<fNProcessedByte<<endl;
    
		 cout <<"N processed byte = "<<fNProcessedByte<<endl;
		for (int i=0; i<fNProcessedByte; i++){
			printf("Byte %d ->  %04x\n",i,fDataIn[i]);
	
	}

	SetIDLE();

	//ftstatus = FT_Purge(fthandle, FT_PURGE_RX | FT_PURGE_TX);

    return ftstatus;
}

int USBCom :: ReadData (unsigned char *read_command, int command_length, unsigned char *data){

	fNProcessedByte = 0;

	//ftstatus = FT_Purge(fthandle, FT_PURGE_RX | FT_PURGE_TX);

	SendStart();
    
    for (int i=0; i<command_length; i++){
        
		if (i==2) SendSR();
        WriteByte(read_command[i]);
        
    }

	//Set IDLE to read data
	for (int i=0; i<2; i++){
    fDataOut[fNWriteByte++] = PINS;
    fDataOut[fNWriteByte++] = 0xF8;
    fDataOut[fNWriteByte++] = PIN_DIR_IN;
	}

	ReadByte();

	SendNACK();
    SendStop();

	ftstatus = FT_Write(fthandle, fDataOut, fNWriteByte, &fNProcessedByte);

	int timeout = TIMEOUT;
	fNReadByte = 0;

	while (timeout >= 0 && fNReadByte < 1 && ftstatus==FT_OK){ 
		ftstatus = FT_GetQueueStatus(fthandle, &fNReadByte);
		timeout--;
		Sleep(1);
	}
    
    if (ftstatus!=FT_OK || fNReadByte<4) cout <<"Not able to read ACKs and data (2): "<<fNReadByte<<endl;
    

			fNProcessedByte = 0;

			timeout = TIMEOUT;

			while (timeout >= 0 && fNProcessedByte < fNReadByte && ftstatus==FT_OK){ 
			ftstatus = FT_Read(fthandle, fDataIn, fNReadByte, &fNProcessedByte);
			timeout--;
			Sleep(1);
			}

		 if (ftstatus!=FT_OK || fNProcessedByte<3) cout <<"Not able to read ACKs and data (3): "<<fNProcessedByte<<endl;
    
		 cout <<"N processed byte = "<<fNProcessedByte<<endl;

		for (int i=0; i<fNProcessedByte; i++) printf("Byte %d ->  %04x\n",i,fDataIn[i]);

		if (fNProcessedByte > 3) data[0] = fDataIn[3];

	SetIDLE();

	//ftstatus = FT_Purge(fthandle, FT_PURGE_RX | FT_PURGE_TX);

    return fNProcessedByte-3;

}

int USBCom :: SetCBUS (int n){

	fNWriteByte = 0;
	fDataOut[fNWriteByte++] = PINS_AC;
	fDataOut[fNWriteByte++] = (0x0 | (0x1  << n));
	fDataOut[fNWriteByte++] = 0xFF;
	
	ftstatus = FT_Write(fthandle, fDataOut, fNWriteByte, &fNProcessedByte);

	if (ftstatus != FT_OK) return 1;
	return 0;

}