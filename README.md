# fdd
This is a linux project for fatigue driving detection,which aims to monitor the driver's fatigue driving behavior,including distraction,yawn,frequent yawn,sleepy, by face analysis.<br>

[![Travis](https://img.shields.io/travis/rust-lang/rust.svg)]()
[![Opencv](https://img.shields.io/badge/Opencv-v3.1-brightgreen.svg)](https://opencv.org/)
[![Caffe](https://img.shields.io/badge/Caffe-v1.0-brightgreen.svg)](http://caffe.berkeleyvision.org/)
[![libconfig](https://img.shields.io/badge/libconfig-v1.7.2-brightgreen.svg)](https://github.com/hyperrealm/libconfig)
[![Packagist](https://img.shields.io/packagist/l/doctrine/orm.svg)]()


## Table of Contents
* [Requirements](#Requirements)
* [Usage](#Usage)
* [Errors May Occur](#Errors-May-Occur)
* [License](#License)

## <a name='Requirements'>Requirements</a>
The project requires the following packages to run:
* Opencv(above v3.0) <https://opencv.org/>
* Caffe <http://caffe.berkeleyvision.org/>
* libconfig++ <https://github.com/hyperrealm/libconfig><br>

You can visit their official website to get source code and learn how to compile and install these packages.

## <a name='Usage'>Usage</a>
1. Compile and install Opencv,Caffe,libconfig++.
2. Clone the fdd_linux repository.
3. Switch the compile options ,according whether the system has screen or not, whether the system can play sound or not ,whether you need to compile tools or not, by editing top level CMakeLists.txt.
```
  cd fdd_linux
  vim CMakeLists.txt
```
 If you switched play sound option on,you should install sox at first.
```
   sudo apt-get install sox
```
4. Compile the source code and install the package by following commands 
```
  mkdir build
  cd build
  cmake ..
  make
  sudo make install
```
5. Enter 'configures' folder under project home and edit the configure file 'DriverMonitorGlobalVariables.cfg'
according to your system environment.You must make sure the 'tarFolder' you configured existed.
```
  cd ../configures/
  vim DriverMonitorGlobalVariables.cfg
```
6. Now you can run the DriverMonitor by following commands
```
  cd ../build/DriverMonitor
  ./DriverMonitor ../../configures/DriverMonitorGlobalVariables.cfg
```
  or
```
  ./DriverMonitor ../../configures/DriverMonitorGlobalVariables.cfg 0
```
The second parameter of DriverMonitor is the camera id which is optional.
## <a name='Errors-May-Occur'>Errors May Occur</a>
1. Error: multiple definition of 'typeinfo for caffe::InnerProductLayer <double>'<br>
   Solution: Edit file 'include/CaffeClassifier/caffe_layers_registry.hpp' to comment INSTANTIATE_CLASS code lines of 
   multiple defined layers

## <a name="License">License</a>
fdd_linux is licensed under the [MIT](https://mit-license.org/) license.<br>
Copyright © 2018 Lyfe667




