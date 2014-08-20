TARGET:=FTDI

FTDI_INCLUDE := ../ftd2xx

INCLUDE:= -Iinclude -I$(FTDI_INCLUDE)

CXX := g++ -ggdb -w


ROOTCFLAGS := $(shell root-config --cflags)

CFLAGS := $(ROOTCFLAGS) $(INCLUDE)
LDFLAGS := $(shell root-config --glibs --ldflags) -Llib 


all: com gui prg

gui:
	@echo compiling gui
	@echo $(CXX) $(CFLAGS) src/CFD_MainFrame.cpp src/CFD_Tab.cpp CFD_Frame_Dict.cxx CFD_Tab_Dict.cxx -Wall -fPIC -shared $(LDFLAGS) -o lib/libGui.so -lcom
	@$(CXX) $(CFLAGS) src/CFD_MainFrame.cpp src/CFD_Tab.cpp CFD_Frame_Dict.cxx CFD_Tab_Dict.cxx -Wall -fPIC -shared $(LDFLAGS) -lcom -o lib/libgui.so 

com:
	@echo Compiling com
	@ echo $(CXX) $(CFLAGS) src/USBCom.cpp src/CFD.cpp  -Wall -fPIC -shared $(LDFLAGS) -lftd2xx -o lib/libcom.so 
	@$(CXX) $(CFLAGS) src/USBCom.cpp src/CFD.cpp  -Wall -fPIC -shared $(LDFLAGS) -lftd2xx -o lib/libcom.so

prg:
	@echo Compiling main
	@echo $(CXX) $(CFLAGS) src/main.cpp $(LDFLAGS) -lcom -lgui -lftd2xx -o $(TARGET).o
	@$(CXX) $(CFLAGS) src/main.cpp $(LDFLAGS) -lcom -lgui -lftd2xx -o $(TARGET).o
