#ifndef DETECTNETCONTROLLER_H_
#define DETECTNETCONTROLLER_H_

#include <stdio.h>
#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <string>
#include "../util/detectnet-camera.h"


#define TARGET_BB_IN_SORTED_ARRAY 0

class DetectNetController {
    public:
        DetectNetController(int defaultCamPort1, 
                                                 int camPort2, 
                                                 std::string model);
        virtual ~DetectNetController();

        enum class CupOrientation {
                VERTICAL=0,
                HORIZONTAL=1,
                UKNOWN=2
        };

        enum class ClassID{
                UNKNOWN=-1,
                CUP=0,
                TRASHCAN=1
        };

        //Structure of Unsorted Bounding Box: [x1][y1][x2][y2] (bottom left: x1, y1; top right: x2, y2)

        //Structure of Sorted Bounding Box: [x1][y1][x2][y2] (bottom left: x1, y1; top right: x2, y2), ClassID
        std::vector<std::array<float, 5>> SortBBArrayByTargetDistance();
        std::vector<std::array<float, 5>> bbArraySorted;

        //Thread Control Functions 
        void JoinDetectThread();

        //Functions that read values from detectnet-controller.cpp
        void ReadCameraResolution();
        float** ReadUnsortedBBArray();
        volatile int* ReadNumberOfDetectedBB();
        bool ReadStopSignal();
    
        void SwitchCameras();

        float GetCenterXFromBB(std::array<float, 5> bb);
        float GetCenterYFromBB(std::array<float, 5> bb);
        bool IsDetectNetReady();
        
        //first parameter is the default camera to start with
        void SetCameraPorts(int bottom_source, int top_source);
        bool IsCurrentCamBottomCam();

        float GetCameraWidth();
        float GetCameraHeight();
        float GetCameraCenterX();
        float GetCameraCenterY();
        
        void SetDetectNetLoopLock(bool state);
        bool GetDetectNetLoopLock();

        std::array<float, 5> GetTargetBB();
        std::array<float, 5> GetTargetBB(DetectNetController::ClassID classID);
        
        float GetAreaOfTargetBB();
        float GetErrorXOfTargetBB();
        float GetErrorXOfTargetBB(float offset);
        float GetErrorYOfTargetBB();
        int GetClassIDFromUnsortedBBNum(int bbNum);
        DetectNetController::ClassID GetClassIDFromSortedBB(int bbNum);
        DetectNetController::ClassID ConvertIntToClassID(int int_class); 

        DetectNetController::CupOrientation GetCupOrientation();

    private:
        float* GetConfCPU();

        float** bbArrayUnsorted;
        volatile int numberOfDetectedBB;

        float cameraCenterX;
        float cameraCenterY;
        float cameraWidth;
        float cameraHeight;

        float GetDistanceFromTwoPoints(float x1, float y1, float x2, float y2);

        //Thread Control
        void runThread();
        std::thread* detectNetThread;

        //Arguments
        std::string m_model;
};

#endif

