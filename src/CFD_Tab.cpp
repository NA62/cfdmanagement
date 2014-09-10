//
//  CFD_Tab.cpp
//  FTDI_COM
//
//  Created by riccardo aliberti on 25/07/14.
//  Copyright (c) 2014 riccardo aliberti. All rights reserved.
//

#include "CFD_Tab.h"

#include <fstream>
#include <sstream>
#include <string>
#include <TString.h>

#include <USBCom.h>
#include <CFD.h>
#include <vector>

#include <TGWindow.h>
#include <TRootEmbeddedCanvas.h>
#include <TGListBox.h>
#include <TGComboBox.h>
#include <TGMenu.h>
#include <TGTextView.h>

#include <TGTextEntry.h>
#include <TGTextEdit.h>

using namespace std;

ClassImp(CFD_Tab);

CFD_Tab :: CFD_Tab (TGWindow* parent) : TGCompositeFrame(parent)
{
    parent -> Resize(1024, 800);
    TGVerticalFrame *MainFrame = new TGVerticalFrame(this, 1024,800, kChildFrame);
    AddFrame(MainFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    TGVerticalFrame *VFrame = new TGVerticalFrame (MainFrame,150,150,kFixedHeight);
    MainFrame -> AddFrame(VFrame, new TGLayoutHints(kLHintsExpandX));
    
    TGGroupFrame *BoardFileFrame = new TGGroupFrame (VFrame,"Board List File",kHorizontalFrame);
    VFrame->AddFrame(BoardFileFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsTop));
    
    fBoardFile = new TGTextEdit(BoardFileFrame,50,50);
    BoardFileFrame -> AddFrame(fBoardFile, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    fBoardFile -> SetText(new TGText(BoardListFile));
    
    fReadBoardFile = new TGTextButton(BoardFileFrame,"Read",fGen.GetID());
    BoardFileFrame -> AddFrame(fReadBoardFile, new TGLayoutHints(kLHintsExpandY | kLHintsRight));
    fReadBoardFile -> Connect("Clicked()", "CFD_Tab", this, "ReadBoardListFile()");
    
    
    TGGroupFrame *CFDFileFrame = new TGGroupFrame (VFrame,"CFD Configuration File",kHorizontalFrame);
    VFrame->AddFrame(CFDFileFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsBottom));
    
    fCFDStructFile = new TGTextEdit(CFDFileFrame,50,50);
    CFDFileFrame -> AddFrame(fCFDStructFile, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    fCFDStructFile -> SetText(new TGText(CFDStructFile));
    
    fReadCFDStruct = new TGTextButton(CFDFileFrame,"Read",fGen.GetID());
    CFDFileFrame -> AddFrame(fReadCFDStruct, new TGLayoutHints(kLHintsExpandY | kLHintsRight,1,1,1,1));
    fReadCFDStruct -> Connect("Clicked()", "CFD_Tab", this, "ReadCFDStructFile()");
    
    
    TGHorizontalFrame *HFrame3 = new TGHorizontalFrame(MainFrame,50,50,kChildFrame);
    MainFrame -> AddFrame(HFrame3, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    TGGroupFrame *CFDSelection = new TGGroupFrame(HFrame3,"CFD Selection", kHorizontalFrame | kChildFrame);
    HFrame3 -> AddFrame(CFDSelection, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    TGGroupFrame *BoardSelection = new TGGroupFrame(CFDSelection,"Boards", kChildFrame);
    CFDSelection -> AddFrame(BoardSelection, new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY));
    
    fBoardList = new TGListBox(BoardSelection,fGen.GetID(),kChildFrame);
    BoardSelection -> AddFrame(fBoardList, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    fBoardList -> Connect("Selected(int)", "CFD_Tab", this, "AddBoardCFDs()");
    
    
    TGGroupFrame *ChannelSelection = new TGGroupFrame(CFDSelection,"CFDs", kChildFrame);
    CFDSelection -> AddFrame(ChannelSelection, new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY));
    
    fCFDList = new TGListBox(ChannelSelection,fGen.GetID(), kChildFrame);
    ChannelSelection -> AddFrame(fCFDList, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    fCFDList -> Connect("Selected(int)", "CFD_Tab", this,"AddCFDDACs()");

    
    
    TGHorizontalFrame *SettingFrame = new TGHorizontalFrame(MainFrame, kChildFrame);
    MainFrame->AddFrame(SettingFrame, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY|kLHintsCenterX));
    
    TGGroupFrame *CFDFrame = new TGGroupFrame(SettingFrame,"CFD Values",kChildFrame);
    SettingFrame -> AddFrame(CFDFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    TGHorizontalFrame *ThrFrame = new TGHorizontalFrame (CFDFrame,50,15,kChildFrame);
    CFDFrame -> AddFrame(ThrFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsCenterX));
    
    TGTextView *ThrView = new TGTextView (ThrFrame, 30, 15, "Thr",kChildFrame);
    ThrFrame -> AddFrame(ThrView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    fDACName[0] = new TGTextView(ThrFrame);
    ThrFrame -> AddFrame(fDACName[0], new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    fValList[0] = new TGComboBox(ThrFrame,3);
    ThrFrame -> AddFrame(fValList[0], new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    fValList[0] -> Connect("Selected(int)", "CFD_Tab",this,"ThrChanged()");
    fValList[0] -> SetEnabled(0);

    
    TGHorizontalFrame *WidthFrame = new TGHorizontalFrame (CFDFrame,50,15,kChildFrame);
    CFDFrame -> AddFrame(WidthFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsCenterX));
    
    TGTextView *WidthView = new TGTextView (WidthFrame, 30, 15, "Width",kChildFrame);
    WidthFrame -> AddFrame(WidthView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    fDACName[1] = new TGTextView(WidthFrame);
    WidthFrame -> AddFrame(fDACName[1], new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    fValList[1] = new TGComboBox(WidthFrame,fGen.GetID());
    WidthFrame -> AddFrame(fValList[1], new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    fValList[1] -> Connect("Selected(int)", "CFD_Tab",this,"WidthChanged()");
    fValList[1] -> SetEnabled(0);
    
    
    TGHorizontalFrame *OffFrame = new TGHorizontalFrame (CFDFrame,50,15,kChildFrame);
    CFDFrame -> AddFrame(OffFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsCenterX));
    
    TGTextView *OffView = new TGTextView (OffFrame, 30, 15, "Offset",kChildFrame);
    OffFrame -> AddFrame(OffView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    fDACName[2] = new TGTextView(OffFrame);
    OffFrame -> AddFrame(fDACName[2], new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    
    fValList[2] = new TGComboBox(OffFrame,3);
    OffFrame -> AddFrame(fValList[2], new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    fValList[2] -> Connect("Selected(int)", "CFD_Tab",this,"OffChanged()");
    fValList[2] -> SetEnabled(0);

    
    TGGroupFrame* ButtonFrame = new TGGroupFrame (SettingFrame,"",kChildFrame);
    SettingFrame -> AddFrame(ButtonFrame, new TGLayoutHints(kLHintsRight | kLHintsCenterY | kLHintsExpandY));
    
    fReadValButton = new TGTextButton (ButtonFrame, "Read Values", fGen.GetID());
    ButtonFrame -> AddFrame(fReadValButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY | kLHintsExpandX | kLHintsCenterY));
    fReadValButton -> Connect("Clicked()", "CFD_Tab", this, "ReadDACVal()");
    fReadValButton -> SetEnabled(0);
    
    fSetValButton = new TGTextButton (ButtonFrame, "Set Values", 104);
    ButtonFrame -> AddFrame(fSetValButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY | kLHintsExpandX | kLHintsCenterY));
    fSetValButton -> Connect("Clicked()", "CFD_Tab", this, "SetDACVal()");
    fSetValButton -> SetEnabled(0);
    
    for (int i=0; i<=255; i++){
        fValList[0] -> AddEntry(Form("%d",i), i);
        fValList[1] -> AddEntry(Form("%d",i), i);
        fValList[2] -> AddEntry(Form("%d",i), i);
    }
    
}





CFD_Tab :: ~CFD_Tab (){};

void CFD_Tab :: ReadFiles (){
    
    int NB = ReadBoardListFile();
    int NCFD = ReadCFDStructFile();
    
}

int CFD_Tab :: ReadBoardListFile(){
    
    TString val = fBoardFile->GetText()->AsString();
    ifstream infile (val.Data());
    
    fBoardList -> RemoveAll();
    
    if (fNBoards>0)
        for (int i=0; i<fNBoards; i++) delete fBoardPorts[i];
    fNBoards = 0;
    
    TString BoardName, SerialNumber;
    
    vector <TString> SN;
    vector <TString> Name;
    
    while (infile >> BoardName >> SerialNumber) {
        Name.push_back(BoardName);
        SN.push_back(SerialNumber);
        
        BoardName = SerialNumber = "";
        
    }
    infile.close();
    
    fNBoards = (int) SN.size();
    
    if (fNBoards<1) {
        cout <<"Board File Empty"<<endl;
        return 0;
    }
    
    fBoardPorts = new USBCom* [fNBoards];
    
    for (int i=0; i<fNBoards; i++){
        fBoardPorts[i] = new USBCom(/*SN[i]*/);
        if (fBoardPorts[i]->IsInitialised())
        fBoardList -> AddEntry(Name[i],i);
        
    }
    
    return fBoardList -> GetNumberOfEntries();
}

int CFD_Tab :: ReadCFDStructFile(){

    TString val = fCFDStructFile->GetText()->AsString();
    cout <<"CFD File = "<<val.Data()<<endl;
    
    ifstream infile (val.Data());
    
    fCFDList -> RemoveAll();
    
    if (fNCFD>0) delete [] fCFDs;
    fNCFD = 0;
    
    CFD_data data;
    vector <CFD_data> CFD_list;
    
    unsigned int int_addr;
    unsigned char addr;
    int Reg, CFD_N, Thr, CBUS;
    TString addr_str, name;
    
    
    while (infile >> addr_str >> Reg >> CFD_N >> Thr >> CBUS >> name ) {
        
        cout <<"Address string = "<<addr_str.Data()<<endl;
        
        stringstream ssaddr;
        ssaddr << hex << addr_str.Data();
        
        cout <<"Stringstream address "<<ssaddr.str().c_str()<<endl;
        
        ssaddr >> hex >> int_addr;
        addr = (unsigned char) int_addr;
        
        cout <<"Found DAC "<<Form("%x %d %d %d %d %s",addr,Reg,CFD_N,Thr,CBUS,name.Data())<<endl;
        
        if (CFD_N<0){
            name = addr_str = "";
            continue;
        }
        
        bool found = false;
        for (int i=0; i<CFD_list.size(); i++){
            if (CFD_list[i].CFD_Number != CFD_N) continue;
            CFD_list[i].CFD_DAC_Number[Thr] = Reg;
            CFD_list[i].CFD_DAC_Name[Thr] = name;
            CFD_list[i].CFD_DAC_Addr[Thr] = addr;
            found = true;
        }
        
        if (!found){
            data.CFD_DAC_Addr[Thr] = addr;
            data.CFD_DAC_Name[Thr] = (char *) name.Data();
            data.CFD_DAC_Number[Thr] = Reg;
            data.CFD_Number = CFD_N;
            data.CFD_CBUS = CBUS;
            CFD_list.push_back(data);
        }
     
        name = addr_str = "";
    }
    
    fNCFD = (int) CFD_list.size();
    
    if (fNCFD<1) {
        cout <<"CFD File Empty"<<endl;
        return 0;
    }
    
    fCFDs = new CFD* [fNCFD];
    for (int i=0; i<fNCFD; i++){
        fCFDs[i] = new CFD (nullptr, CFD_list[i].CFD_DAC_Addr, CFD_list[i].CFD_DAC_Number, CFD_list[i].CFD_CBUS);
        fCFDs[i] -> SetDACName(0, CFD_list[i].CFD_DAC_Name[0]);
        fCFDs[i] -> SetDACName(1, CFD_list[i].CFD_DAC_Name[1]);
        fCFDs[i] -> SetDACName(2, CFD_list[i].CFD_DAC_Name[2]);
        fCFDList -> AddEntry(Form("%d",i), i);
    }
    
    Resize();
    return fCFDList->GetNumberOfEntries();
}

int CFD_Tab :: ConnectBoards (){return 0;};

int CFD_Tab :: AddBoardCFDs(){

    int SelBoard = fBoardList -> GetSelected();
    
    if (SelBoard >= 0) fSelectedBoard = fBoardPorts[SelBoard];
    else if (fBoardList->GetNumberOfEntries()) {
        fSelectedBoard = fSelectedBoard = fBoardPorts[0];
        fBoardList -> Select(0);
    }
    else return 0;
    
    int SelCFD = fCFDList -> GetSelected();
    
    if (SelCFD >= 0) fSelectedCFD = fCFDs[SelCFD];
    else if (fCFDList->GetNumberOfEntries()){
        fSelectedCFD = fCFDs[0];
        fCFDList -> Select(0);
    }
    else return 0;
    
    for (int i=0; i<3; i++) fValList[i]->SetEnabled(1);
    fSetValButton -> SetEnabled(1);
    fReadValButton -> SetEnabled(1);
    
    return 0;
};

int CFD_Tab :: AddCFDDACs(){
    
    int SelCFD = fCFDList -> GetSelected();
    
    if (SelCFD >= 0) fSelectedCFD = fCFDs[SelCFD];
    else if (fCFDList->GetNumberOfEntries()){
        fSelectedCFD = fCFDs[0];
        fCFDList -> Select(0);
    }
    else return 0;
    
    int SelBoard = fBoardList -> GetSelected();
    
    if (SelBoard >= 0) fSelectedBoard = fBoardPorts[SelBoard];
    else if (fBoardList->GetNumberOfEntries()) {
        fSelectedBoard = fSelectedBoard = fBoardPorts[0];
        fBoardList -> Select(0);
    }
    else return 0;
    
    
    fSetValButton -> SetEnabled(1);
    fReadValButton -> SetEnabled(1);
    for (int i=0; i<3; i++){
        fValChanged[i] = true;
        fValList[i] -> SetEnabled(1);
        fDACName[i] -> SetText(new TGText(fSelectedCFD->GetDACName(i)));
    }
    
    GetDACVal();
    Resize();

    return 0;
};

void CFD_Tab :: ReadDACVal(){
    
    for (int i=0; i<3; i++) fValChanged[i] = false;
    
    GetDACVal();
}

int CFD_Tab :: SetDACVal (){
    
    fSelectedCFD -> SetUSBPort(fSelectedBoard);
    
    for (int i=0; i<3; i++){
        if (!fValChanged[i]) continue;
        fValList[i] -> RemoveEntry(1000);
        
        int val = fValList[i] -> GetSelected();
        int res = fSelectedCFD -> SetDAC(i,val);
        
        if (res == 0) fValChanged[i] = false;
        
    }
    
    fSelectedCFD -> SetUSBPort(nullptr);
    
    GetDACVal();
    
    return 0;
};


int CFD_Tab :: GetDACVal (){

    fSelectedCFD -> SetUSBPort(fSelectedBoard);
    
    for (int i=0; i<3; i++) {
        if (!fValChanged[i]) continue;
        fValList[i] -> RemoveEntry(1000);
        
        unsigned int res = fSelectedCFD -> GetDAC(i);
        
        if (res > 0xff){
            
            if (res > 0xfff) fValList[i]->AddEntry("NACK", 1000);
            else fValList[i]->AddEntry("No Data", 1000);
            
        }
        
        else fValList[i]->Select(res);
        
        fValChanged[i] = false;
    }


    return 0;
};

void CFD_Tab :: ThrChanged (){
    fValChanged[0] = true;
}

void CFD_Tab :: WidthChanged (){
    fValChanged[1] = true;
}

void CFD_Tab :: OffChanged (){
    fValChanged[2] = true;
}
