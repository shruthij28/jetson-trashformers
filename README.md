# jetson-trashformers

Repository for the Trashformers NVIDIA Embedded Intern Team!

### Who are the Trashformers?
The Trashformers are a group of high school summer interns at NVIDIA. The Trashformer is the humanoid robot from Robotis that the team is working with. 

### What is this project about?
The goal of this project is to use neural networks to train the Robotis BiolidGP (named Trashformer by the team) to detect trash and throw it away in trash cans, effectively keeping the office environment clean and free of trash. As of July 12, the Trashformer is able to detect cups on the ground and pick them up. In the coming weeks, the team plans to train the robot to throw away cups in trash cans. 

### How can I run this project?
```
git clone https://github.com/NVIDIA-Jetson/jetson-trashformers.git
make
sh runDetect.sh
```
These three commands will clone the project to the computer so that it can be run and/or edited to the the user's liking, compile the program, and allow the user to run the program to detect cups. 

### What is CupNet?
CupNet is the neural network the Trashformers have created in order to have the robot detect cups. It has been trained on images of white and red cups, as well as false positives to make the model more accurate in recognizing cups. This neural network has been created and trained on [NVIDIA DIGITS](https://developer.nvidia.com/digits) using the Caffe framework. The Trashformers used the help of [Dusty's Jetson Inference tutorial](https://github.com/dusty-nv/jetson-inference) to learn more about using DIGITS and creating their own neural network. 

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


