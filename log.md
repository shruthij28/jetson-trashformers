###log friday 6/30/17:
I successfully got detectnet-camera working. I did the following:
1. edit detectnet-camera.cpp with the functions that I want
2. create a header file that our program links to (I made a copy and put it in ./util/)
3. in jetson-inference/build: run 'cmake ..' and 'make'
4. copy the libjetson-inference.so file from ~/jetson-inference/build/aarch/lib/ to ./lib
5. edit the CMakeLists.txt file in ~/jetson-inference and add the flag -fPIC
6. go to jetson-inference/build/detectnet-camera/CMakeFile/detectnet-camera.dir/
7. run: 
```
g++ -shared -o libdetectnet-camera.so detectnet-camera.cpp.o
```
8. cp the resulting so file to ./lib
9. make sure Makefile includes this new library

next, we need to include the cuda library files (/usr/local/cuda/lib64) to our makefile:
```
LIBDIRS     += -Llib -L/usr/local/cuda/lib64
LDFLAGS     = $(LIBDIRS) -ldetectnet-camera -ljetson-inference -lcudart -lzgb -ldxl_sbc_cpp
```
Finally, run ./main with the flags you would have given detectnet:
```
 ./main --prototxt=$NET/deploy.prototxt --model=$NET/snapshot_iter_1240.caffemodel --input_blob=data --output_cvg=coverage --output_bbox=bboxes
```
