#include "DetectNetController.h"

DetectNetController::DetectNetController(int defaultCamPort1, 
                                         int camPort2, 
                                         std::string model){
    //Run the DetectNet Program
    m_model = model;
    
    SetCameraPorts(defaultCamPort1, camPort2);
    detectNetThread = new std::thread(&DetectNetController::runThread, this);
}

DetectNetController::~DetectNetController(){

}

//THREAD FUNCTIONS
void DetectNetController::runThread(){
    runDetectNet(m_model);
}

void DetectNetController::JoinDetectThread(){
    detectNetThread->join();
}

void DetectNetController::ReadCameraResolution(){ 
    cameraCenterX = (float)(getCameraWidth()) / 2.0;
    cameraCenterY = (float)(getCameraHeight()) / 2.0;
    cameraWidth = getCameraWidth();
    cameraHeight = getCameraHeight();
    
    //print camera dimensions
    std::cout << "CAM_CENTER_X" << cameraCenterX << std::endl;
    std::cout << "CAM_CENTER_Y" << cameraCenterY << std::endl;
}

//ARRAY SORTING
std::vector<std::array<float, 5>> DetectNetController::SortBBArrayByTargetDistance(){ 
        bbArrayUnsorted = ReadUnsortedBBArray();
        numberOfDetectedBB = *ReadNumberOfDetectedBB();
        
        bbArraySorted.clear();
        
        printf("Found %i Bounding Boxes.\n", numberOfDetectedBB);
        
        int boxNum = 0; //used to keep track of which box we are on in the unsorted bb array 
        for(int i=0; i<numberOfDetectedBB * 4; i+=4){
            float* bb = bbArrayUnsorted[0];
            std::array<float, 5> arr = { bb[i], bb[i+1], bb[i+2], bb[i+3], (float)GetClassIDFromUnsortedBBNum(boxNum)};
            bbArraySorted.push_back(arr);
 
            //printf("BB #%i: (X1: %f, Y1: %f), (X2:%f, Y2: %f), classID: %i\n", boxNum, arr[0], arr[1], arr[2], arr[3], classID);
            boxNum++;
        }

        if(numberOfDetectedBB > 0){
            //Sort array based on the bounding boxes' distances from center of camera
            std::sort(bbArraySorted.begin(), bbArraySorted.end(), [this](std::array<float, 5> a, std::array<float, 5> b) {
                float tmpCenterX1 = this->GetCenterXFromBB(a);
                float tmpCenterX2 = this->GetCenterXFromBB(b);
                
                float tmpCenterY1 = this->GetCenterYFromBB(a);
                float tmpCenterY2 = this->GetCenterYFromBB(b);

                float distance1 = this->GetDistanceFromTwoPoints(tmpCenterX1, tmpCenterY1, this->cameraCenterX, this->cameraCenterY);
                float distance2 = this->GetDistanceFromTwoPoints(tmpCenterX2, tmpCenterY2, this->cameraCenterX, this->cameraCenterY);
                
                return distance1 < distance2;
            });
        }        

        std::cout << std::endl;

        return bbArraySorted;
}

std::array<float, 5> DetectNetController::GetTargetBB(){
    std::vector< std::array<float, 5> > sortedArray = SortBBArrayByTargetDistance();
    if(sortedArray.size() > 0) return sortedArray[0];
    else return std::array<float, 5>();
}

//assumes array has been sorted
std::array<float, 5> DetectNetController::GetTargetBB(DetectNetController::ClassID classID){
    std::vector< std::array<float, 5> > sortedArray = bbArraySorted;
    std::array<float, 5> emptySet;
    emptySet.fill(-1);
    if(sortedArray.size() == 0) return emptySet;
    for(int i=0; i<(int)sortedArray.size(); i++) {
        if(ConvertIntToClassID((int)(sortedArray[i][4])) == classID)
            return sortedArray[i]; 
    }
    return emptySet;
}

float DetectNetController::GetAreaOfTargetBB(){
    if(bbArraySorted.size() < 1) return -1;
    std::array<float, 5> bb = bbArraySorted[0];
    return (bb[2]-bb[0]) * (bb[3]-bb[1]);
}

float DetectNetController::GetDistanceFromTwoPoints(float x1, float y1, float x2, float y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

//RETURN FUNCTIONS FOR detectnet-camera.cpp 
float** DetectNetController::ReadUnsortedBBArray(){
    return getBoundingBoxArray();
}

volatile int* DetectNetController::ReadNumberOfDetectedBB(){
    return getNumBoundingBox();
}

/**
 * Returns true if CTRL + C is pressed
 */
bool DetectNetController::ReadStopSignal(){
    return getStopSignal();
}

bool DetectNetController::GetDetectNetLoopLock(){
    return getLoopLock();
}

void DetectNetController::SetDetectNetLoopLock(bool state){
    setLoopLock(state);
}

float DetectNetController::GetCenterXFromBB(std::array<float, 5> bb) {
    if(!bb.empty()){
        printf("GetCenterXFromBB: bb[0] = %0.0f, bb[2] = %0.0f \n", bb[0], bb[2]);
        return (bb[0] + bb[2]) / 2.0;
    }
    else return -1;
}

float DetectNetController::GetCenterYFromBB(std::array<float, 5> bb) {
    if(!bb.empty()){ 
        printf("GetCenterXFromBB: bb[1] = %0.0f, bb[3] = %0.0f \n", bb[1], bb[3]);
        return (bb[1] + bb[3]) / 2.0;   
    }
    else return -1;
}

float DetectNetController::GetErrorXOfTargetBB(float offset) {
   if(bbArraySorted.size() < 1) return 0.0;
   float cX = GetCenterXFromBB(bbArraySorted[0]);
   if(cX == -1) return 0.0;
   return cX - GetCameraCenterX() - offset; 
}

float DetectNetController::GetErrorXOfTargetBB(){
    return GetErrorXOfTargetBB(0.0);
}

float DetectNetController::GetErrorYOfTargetBB() {
   if(bbArraySorted.size() < 1) return 0.0;
   float cY = GetCenterYFromBB(bbArraySorted[0]);
   if(cY == -1) return 0.0;
   return cY - GetCameraCenterY(); 
}

bool DetectNetController::IsDetectNetReady(){
    return isCameraLoaded();
}

void DetectNetController::SetCameraPorts(int bottom_source, int top_source){
    //command to read video devices: ls -ltrh /dev/video*
    setCameraPorts(bottom_source, top_source);
}

void DetectNetController::SwitchCameras(){
    switchCamera();
}

bool DetectNetController::IsCurrentCamBottomCam(){
    return isCurrentCamBottomCam();
}

float DetectNetController::GetCameraWidth(){
    return cameraWidth;
}

float DetectNetController::GetCameraHeight(){
    return cameraHeight;
}

float DetectNetController::GetCameraCenterX(){
    return cameraCenterX;
}

float DetectNetController::GetCameraCenterY(){
    return cameraCenterY;
}

int DetectNetController::GetClassIDFromUnsortedBBNum(int bbNum){
    float* confCPU = GetConfCPU();
    return (int)confCPU[bbNum*2+1];//process to acquire class # obtained from detectnet-camera.cpp
}

DetectNetController::ClassID DetectNetController::GetClassIDFromSortedBB(int bbNum){
    return ConvertIntToClassID((float)bbArraySorted[bbNum][4]);
}

DetectNetController::ClassID DetectNetController::ConvertIntToClassID(int int_class){
    return static_cast<DetectNetController::ClassID>(int_class);
/*
    switch(int_class){
        case UNKNOWN:
            convertedClass = DetectNetController::ClassID::UNKNOWN;
            break;
        case CUP:
            convertedClass = DetectNetController::ClassID::CUP;
            break;
        case TRASHCAN:
            convertedClass = DetectNetController::ClassID::TRASHCAN;
            break;
        default: 
            convertedClass = DetectNetController::ClassID::UNKNOWN; 
    }
 */   
}

float* DetectNetController::GetConfCPU(){
    return getConfCPU();
}

DetectNetController::CupOrientation DetectNetController::GetCupOrientation(){
    std::array<float, 5> targetCup = GetTargetBB();
    if(targetCup.empty()) return CupOrientation::UKNOWN;
    float width = targetCup[2] - targetCup[0];
    float height = targetCup[3] - targetCup[1];
    if(width > height) return CupOrientation::HORIZONTAL;
    return CupOrientation::VERTICAL;
}
