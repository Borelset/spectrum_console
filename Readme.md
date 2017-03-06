##安装opencv
###编译环境
```
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
```
###编译   
下载opencv
```
wget https://github.com/opencv/opencv/archive/3.2.0.zip
```
下载并解压opencv后，进入opencv的文件夹
```
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=release -D CMAKE_INSTALL_PREFIX=/usr/local ..
make
make install
```
###配置
```
sudo gedit /etc/ld.so.conf.d/opencv.conf
```
在opencv.conf中写入
```
/usr/local/lib
```
        
```
sudo ldconfig
sudo gedit /etc/bash.bashrc
```
在bash.bashrc的最后加入
```
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH
```

