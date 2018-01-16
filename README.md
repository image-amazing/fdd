# fdd_linux
This is a linux project for fatigue driving detection,which aims to monitor the driver's fatigue driving behavior,including distraction,
yawn,frequent yawn,sleepy, by face analysis.

## Table of Contents
* [Requirements](#Requirements)
* [Usage](#Usage)
* [Errors May Occur](#Errors-May-Occur)
* [License](#License)

## Requirements
The project requires the following packages to run:
* Opencv(above v3.0) <https://opencv.org/>
* Caffe <http://caffe.berkeleyvision.org/>
* libconfig++ <https://github.com/hyperrealm/libconfig><br>

You can visit their official website to get source code and learn how to compile and install these packages.

## Usage
1. Compile and install Opencv,Caffe,libconfig++.
2. Clone the fdd_linux repository.
3. Switch the compile options ,according the system has screen or not,the system can play sound or not ,you need to
   compile tools or not, by editing top level CMakeLists.txt.
```
  cd fdd_linux
  vim CMakeLists.txt
```
 If you switched play sound on,you should install sox at first.
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
## Errors May Occur
1. Error: multiple definition of 'typeinfo for caffe::InnerProductLayer <double>'<br>
   Solution: Edit file 'include/CaffeClassifier/caffe_layers_registry.hpp' to comment INSTANTIATE_CLASS code lines of 
   multiple defined layers

## License
fdd_linux is licensed under the [MIT](https://mit-license.org/) license.<br>
Copyright © 2018 Lyfe667




