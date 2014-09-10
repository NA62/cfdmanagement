//
//  CFD_Tab.h
//  FTDI_COM
//
//  Created by riccardo aliberti on 25/07/14.
//  Copyright (c) 2014 riccardo aliberti. All rights reserved.
//

#ifndef __FTDI_COM__CFD_Tab__
#define __FTDI_COM__CFD_Tab__

#include <TGFrame.h>
#include <RQ_OBJECT.h>

#define BoardListFile "./Res/Board.txt"
#define CFDStructFile "./Res/CFD_Test.txt"

class USBCom;
class CFD;

class TGWindow;
class TRootEmbeddedCanvas;
class TGListBox;
class TGComboBox;
class TGTextButton;
class TGPopupMenu;
class TGTextView;


class TGTextEntry;
class TGTextButton;
class TGTextEdit;

struct CFD_data {
    
    int CFD_Number;
    int CFD_DAC_Number[3];
    unsigned char CFD_DAC_Addr[3];
    TString CFD_DAC_Name[3];
    int CFD_CBUS;
    
};

class IDGen {
    
public:
    IDGen () { fID = -1; };
    virtual ~IDGen () { };
    
    int GetID (){ fID++; return fID; };
    
private:
    int fID;
};



class CFD_Tab: public TGCompositeFrame {
    RQ_OBJECT("CFD_Tab")
    
    
    
public:
    CFD_Tab (TGWindow* parent);
    virtual ~CFD_Tab ();
    
    int ReadBoardListFile();
    int ReadCFDStructFile();
    void ReadFiles ();
    
    int ConnectBoards ();
    int AddBoardCFDs();
    int AddCFDDACs();
    
    int SetDACVal ();
    int GetDACVal ();
    void ReadDACVal ();
    
    void ThrChanged ();
    
    void WidthChanged ();
    void OffChanged ();

private:
    TGListBox *fBoardList;
    TGListBox *fCFDList;
    TGListBox *fDACList;
    
    TGComboBox *fValList[3];
    TGTextView *fDACName[3];
    TGTextButton *fReadValButton;
    TGTextButton *fSetValButton;
    
    TGTextEdit *fBoardFile;
    TGTextButton *fReadBoardFile;
    TGTextButton *fReadCFDStruct;
    TGTextEdit *fCFDStructFile;
    
    USBCom **fBoardPorts;
    CFD **fCFDs;
    
    int fNBoards;
    int fNCFD;
    
    bool fValChanged[3];
    USBCom *fSelectedBoard;
    CFD *fSelectedCFD;
    
    IDGen fGen;
    
    ClassDef(CFD_Tab, 1)
};






#endif /* defined(__FTDI_COM__CFD_Tab__) */
