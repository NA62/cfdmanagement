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
