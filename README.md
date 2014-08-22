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

#### Linux+ARM

```bash
wget http://lightput.com/libftd2xx_1.1.12_Raspberry_Pi_Fix.tar.gz -C ftd2xx
tar -xvzf libftd2xx_1.1.12_Raspberry_Pi_Fix.tar.gz
mv "libftd2xx 1.1.12 Raspberry Pi Fix"/ ftd2xx
sudo cp ftd2xx/build/arm926/lib* /usr/local/lib/
sudo ln -sf /usr/local/lib/libftd2xx.so.1.1.12 /usr/local/lib/libftd2xx.so
sudo chmod 0755 /usr/local/lib/libftd2xx.so.1.1.12
sudo cp ftd2xx/*.h /usr/local/include/

# make shure /usr/local/lib is configured in /etc/ld.so.conf.d/*
sudo ldconfig
```
#### Linux
With Linux you can download the offical ftd2xx version here: http://www.ftdichip.com/Drivers/D2XX.htm. The commands are similar to the ARM installation.
