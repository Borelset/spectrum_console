基于轮廓识别的新型二维码   
==================== 
安装opencv
---------
编译环境
```
chmod 775 opencv_setup.sh
./opencv_setup.sh
```
或者
```
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
```   
下载opencv
```
wget https://github.com/opencv/opencv/archive/3.2.0.zip
```
编译
```
unzip 3.2.0.zip
cd opencv-3.2.0
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=release -D CMAKE_INSTALL_PREFIX=/usr/local ..
make
make install
```
配置
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
使用
-----------------
生成二维码   
```
./spectrum -g [text]
```
输入命令后会生成一个code.jpg

识别二维码
```
./spectrum -r [filename]
```
生成范例
------------- 
<img src="https://github.com/Borelset/spectrum/blob/master/code.jpg" width = "300" height = "300" />   
可以修改模板，将四个角上的识别标记替换为其他图形，生成各种各样的二维码，只要该图形的子轮廓层数大于等于6即可。
这样就可以不再对四角上识别标记的具体形状做限制，提高多样性。    
默认使用的模板为template.jpg，替换为template1.jpg或者template2.jpg也可以使用。   
   
感谢
---------------
感谢mersinvald的Reed-Solomon项目提供的里德所罗门算法的实现。
   
本项目中的poly.hpp / gf.hpp / rs.hpp 来自
   
https://github.com/mersinvald/Reed-Solomon
