# Jetson-Trashformers

## Authors
* Michael Chacko
* Shruthi Jaganathan
* Ishan Mitra
* Mark Theis

![Team Pic](https://github.com/shruthij28/jetson-trashformers/blob/master/images/teampic.jpg)

### What is this project about?
The goal of this project is to use neural networks to train the [Robotis BioloidGP](http://en.robotis.com/index/product.php?cate_code=121510) to detect trash and throw it away in trash cans, effectively keeping the office environment clean. 

<!-- add image after resizing![Robot Pic](https://github.com/shruthij28/jetson-trashformers/blob/master/images/robot.jpg) -->

### How can I run this project?
```
git clone https://github.com/NVIDIA-Jetson/jetson-trashformers.git
make
sh runDetect.sh
```
These three commands will clone the project to the computer so that it can be run and/or edited to the the user's liking, compile the program, and allow the user to run the program to pick up trash.

### What is CupNet?
CupNet is the neural network that we have created in order to detect cups. It has been trained on images of white and red cups, as well as false positives to make the model more accurate in recognizing cups. This neural network has been created and trained on [NVIDIA DIGITS](https://developer.nvidia.com/digits) using the Caffe framework. We used the help of [Dusty's Jetson Inference tutorial](https://github.com/dusty-nv/jetson-inference) to learn more about using DIGITS and creating their own neural network.


This graph shows the model's precision, mAP, etc. from the training period.
![DIGITS Graph](https://github.com/shruthij28/jetson-trashformers/blob/master/images/model26.png)

This is the camera's view as it draws bounding boxes around cups using the knowledge it gained from training.
![Cups Detection](https://github.com/shruthij28/jetson-trashformers/blob/master/images/cup_tweet.png)

### Licensing?
See 'licenses' folder for more information.

### Libraries?
See 'lib' folder for the specific files. 
* libdetectnet-camera.so <!-- Rename with the new name for DetectNet Camera-->
  * A shared object library with an edited and compiled version of detectnet-camera.cpp from [Dusty's github](https://github.com/dusty-nv/jetson-inference)
* libdxl_sbc_cpp.so
  * A shared object library for the [Dynamixel servos](https://github.com/ROBOTIS-GIT/DynamixelSDK).
* libjetson-inference.so
  * A shared object library of [Dusty's jetson-inference](https://github.com/dusty-nv/jetson-inference).
* libzgb.so
  * A shared object library to control robot commands via [ZigBee](http://support.robotis.com/en/software/zigbee_sdk/zig2serial/linux.htm). 



<!--
### Add jetson-trashformers/lib to your library path
We will soon figure out a better way to handle these paths. For now, manually add lib via the export command:
```
export LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
```

### To build Zigbee.so files:

For zgb, go to jetson-trashformers/util and run the commands:
```
g++ -c -fpic *.c
g++ -shared -o libzgb.so *.o
```
then move the resulting .so file to jetson-trashformers/lib:
```
mv ./libzgb.so ../lib
```

### Add libraries from detectnet-camera / jetson-inference
See the log.md file for instructions on how we got that to work. Imporoved instructions are in the works.
-->

