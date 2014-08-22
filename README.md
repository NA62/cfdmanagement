cfdmanagement
=============

Management system to change CFD settings via an FTDI controller

## Installation on a Raspberry Pi

### ROOT
```bash
sudo apt-get install libx11-dev libxpm-dev libxft-dev libxext-dev

git clone  http://root.cern.ch/git/root.git
cd root
git checkout v5-34-00-patches
./configure linuxarm --prefix=/usr/local/  --with-x11-libdir=/usr/lib/arm-linux-gnueabihf/ --with-xpm-libdir=/usr/lib/arm-linux-gnueabihf/ --with-xft-libdir=/usr/lib/arm-linux-gnueabihf/ --with-xext-libdir=/usr/lib/arm-linux-gnueabihf/

```

### D2XX
See http://www.ftdichip.com/Drivers/D2XX.htm

For Linux+ARM the installation should look like the following:

```bash
version=1.1.12
wget http://www.ftdichip.com/Drivers/D2XX/Linux/libftd2xx${version}.tar.gz
tar -xvzf libftd2xx${version}.tar.gz
sudo cp release/build/arm926/lib* /usr/local/lib/
sudo cp release/ftd2xx.h /usr/local/include/

# make shure /usr/local/lib is configured in /etc/ld.so.conf.d/*
sudo ldconfig
```

