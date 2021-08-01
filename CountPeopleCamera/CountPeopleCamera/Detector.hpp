//
//  Detector.hpp
//  Darknet을 이용하여 객체를 탐지합니다.
//
//  Created by Printf("HelloWorld!") on 2021/07/29.
//

#ifndef Detector_hpp
#define Detector_hpp

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::dnn;

/* Detector의 매개 변수입니다.
 @param data: 데이터 파일입니다.
 @param confThreshold: 신뢰 임계 값 입니다.
 @param nmsThreshold: 비 최대 억제 임계 값(NMS) 입니다.
 @param width: 네트워크의 입력 이미지 너비입니다.
 @param height: 네트워크의 입력 이미지 높이입니다.
 @param gpu: GPU 사용 여부입니다.
 */
struct DetectorParam
{
    string data;
    float confThreshold;
    float nmsThreshold;
    int width;
    int height;
    bool gpu;
};

// Darknet을 이용하여 객체를 탐지합니다.
class Detector
{
public:
    /* Detector를 생성 후 초기화합니다.
     @param param: Detector의 매개 변수입니다.
     */
    Detector(DetectorParam param);
    
    /* 프레임에서 사람을 탐지합니다.
     @param frame: 탐지할 프레임입니다.
     @return 탐지된 사람의 수 입니다.
     */
    size_t Detect(Mat frame);
    
private:
    Net net;
    
    DetectorParam param;
};

#endif /* Detector_hpp */
