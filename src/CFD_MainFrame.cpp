//
//  CFD_MainFrame.cpp
//  FTDI_COM
//
//  Created by riccardo aliberti on 25/07/14.
//  Copyright (c) 2014 riccardo aliberti. All rights reserved.
//

#include "CFD_MainFrame.h"
#include "CFD_Tab.h"

#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TGTab.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGSlider.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TGStatusBar.h>
#include <TRootEmbeddedCanvas.h>
#include <TG3DLine.h>
#include <TGTextEdit.h>
#include <TGText.h>


ClassImp(CFD_MainFrame)

TGStatusBar* CFD_MainFrame::statusBar_;
TGMainFrame* CFD_MainFrame::fMain;

CFD_MainFrame :: CFD_MainFrame (){
    
    fMain = new TGMainFrame(gClient->GetRoot(),1,1);
    fMain -> Connect("CloseWindow()", "CFD_MainFrame", this, "CloseWindow()");
    
    TGTab* tabMain = new TGTab(fMain,1000,600);
    fMain -> AddFrame(tabMain, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
    
    CFD_Tab *tab = new CFD_Tab(tabMain);
    tabMain -> AddTab("CFD",tab);
    
    statusBar_ = new TGStatusBar(fMain, 240, 18);
	statusBar_->SetName("fStatusBar778");
	fMain->AddFrame(statusBar_,
                    new TGLayoutHints(kLHintsBottom | kLHintsExpandX));
	statusBar_->MoveResize(16, 8, 240, 18);
    
	/*
	 * Settings for the Main Window (title, resize..)
	 */
	fMain->SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
	fMain->SetCleanup(kDeepCleanup);
	fMain->SetWindowName("MUV3 CFD Settings");
	fMain->MapSubwindows();
	fMain->Resize(fMain->GetDefaultSize());
	fMain->MapWindow();
    
}



CFD_MainFrame :: ~CFD_MainFrame(){
    
    fMain -> Cleanup();
    delete fMain;

}


void CFD_MainFrame::CloseWindow() {
	
    std::cout << "Closing application!" << std::endl;
	gApplication->Terminate(0);

}
