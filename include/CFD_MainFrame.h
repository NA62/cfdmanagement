//
//  CFD_MainFrame.h
//  FTDI_COM
//
//  Created by riccardo aliberti on 25/07/14.
//  Copyright (c) 2014 riccardo aliberti. All rights reserved.
//

#ifndef __FTDI_COM__CFD_MainFrame__
#define __FTDI_COM__CFD_MainFrame__

#include <TQObject.h>
#include <RQ_OBJECT.h>

class TGWindow;
class TGMainFrame;
class TGStatusBar;

class CFD_MainFrame {
    RQ_OBJECT("CFD_MainFrame")
    
private:
    static TGMainFrame *fMain;
    static TGStatusBar *statusBar_;
    
public:
    CFD_MainFrame ();
    virtual ~CFD_MainFrame ();
    

    void CloseWindow ();
    
    ClassDef(CFD_MainFrame, 1);
};

#endif /* defined(__FTDI_COM__CFD_MainFrame__) */
