#!/bin/sh

#  rootdict.sh
#  FTDI_COM
#
#  Created by riccardo aliberti on 25/07/14.
#  Copyright (c) 2014 riccardo aliberti. All rights reserved.

rootcint -f CFD_Frame_Dict.cxx -c include/CFD_MainFrame.h include/CFD_Frame_LinkDef.h
rootcint -f CFD_Tab_Dict.cxx -c include/CFD_Tab.h include/CFD_Tab_LinkDef.h