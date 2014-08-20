//
//  main.cpp
//  FTDI_COM
//
//  Created by riccardo aliberti on 03/07/14.
//  Copyright (c) 2014 riccardo aliberti. All rights reserved.
//

//#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "USBCom.h"
#include "CFD.h"

#include "CFD_MainFrame.h"
#include "CFD_Tab.h"

#include <TApplication.h>

using namespace std;

int CFD_Settings () {
    new CFD_MainFrame();
    return 0;
}

int main(int argc, char **argv)
{
    
    TApplication theApp("MUV3 CFD Setting", &argc, argv);
    CFD_Settings();
    theApp.Run();
    
    return 0;
}

