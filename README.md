# jetson-trashformers

Repository for the Trashformers NVIDIA Embedded Intern Team!

### Who are the Trashformers?
The Trashformers are a group of high school summer interns at NVIDIA. The Trashformer is the humanoid robot from Robotis that the team is working with. 

### What is this project about?
The goal of this project is to use neural networks to train the Robotis BiolidGP (named Trashformer by the team) to detect trash and throw it away in trash cans, effectively keeping the office environment clean and free of trash. As of July 12, the Trashformer is able to detect cups on the ground and pick them up. In the coming weeks, the team plans to train the robot to throw away cups in trash cans. 

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


