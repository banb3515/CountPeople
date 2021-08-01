//
//  Detector.cpp
//  Darknet을 이용하여 객체를 탐지합니다.
//
//  Created by Printf("HelloWorld!") on 2021/07/29.
//

#include "Detector.hpp"

// Detector를 생성 후 초기화합니다.
Detector::Detector(DetectorParam param)
{
    // 매개 변수 유효성 확인
    if (param.data == "")
    {
        cout << "# [Detector] data 값이 잘못되었습니다.\n";
        exit(-1);
    }
    else if (param.confThreshold <= 0)
    {
        cout << "# [Detector] confidence-threshold 값이 잘못되었습니다.\n";
        exit(-1);
    }
    else if (param.nmsThreshold <= 0)
    {
        cout << "# [Detector] nms-threshold 값이 잘못되었습니다.\n";
        exit(-1);
    }
    else if (param.width <= 0)
    {
        cout << "# [Detector] width 값이 잘못되었습니다.\n";
        exit(-1);
    }
    else if (param.height <= 0)
    {
        cout << "# [Detector] height 값이 잘못되었습니다.\n";
        exit(-1);
    }
    
    this->param = param;
    
    // 데이터 파일 확장자 설정
    string configFile = param.data;
    configFile.append(".cfg");
    string weightsFile = param.data;
    weightsFile.append(".weights");
    
    // 데이터 파일들이 존재하는지 확인
    if (access(configFile.c_str(), F_OK) != 0)
    {
        cout << "# " << configFile << " 파일을 찾을 수 없습니다.\n";
        exit(-1);
    }
    else if (access(weightsFile.c_str(), F_OK) != 0)
    {
        cout << "# " << weightsFile << " 파일을 찾을 수 없습니다.\n";
        exit(-1);
    }
    
    // Darknet 설정
    net = readNetFromDarknet(configFile, weightsFile);
    if (param.gpu)
    {
#if CUDA == 1
        net.setPreferableBackend(DNN_BACKEND_CUDA);
        net.setPreferableTarget(DNN_TARGET_CUDA);
#elif CUDA == 0
        net.setPreferableBackend(DNN_BACKEND_OPENCV);
        net.setPreferableTarget(DNN_TARGET_OPENCL);
#endif
    }
    else net.setPreferableBackend(DNN_TARGET_CPU);
}

// 프레임에서 사람을 탐지합니다.
size_t Detector::Detect(Mat frame)
{
    Mat blob;
    blobFromImage(frame, blob, 1 / 255.0, Size(param.width, param.height), Scalar(0, 0, 0), true, false);
    net.setInput(blob);
    
    vector<String> names;
    vector<int> outLayers = net.getUnconnectedOutLayers();
    vector<String> layersNames = net.getLayerNames();
                
    names.resize(outLayers.size());
    for (size_t i = 0; i < outLayers.size(); ++i)
        names[i] = layersNames[outLayers[i] - 1];
    
    vector<Mat> outs;
    net.forward(outs, names);
    
    vector<float> confis; // 탐지된 정확도 배열
    vector<Rect> rects; // 탐지된 위치 배열
    
    for (int idx = 0; idx < outs.size(); ++idx)
    {
        float* data = (float*)outs[idx].data;
        
        for (int j = 0; j < outs[idx].rows; ++j, data += outs[idx].cols)
        {
            Mat scores = outs[idx].row(j).colRange(5, outs[idx].cols);
            Point cPoint;
            double confi;
            
            minMaxLoc(scores, 0, &confi, 0, &cPoint);
            
            if (confi > param.confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;
                
                if (left < 0) left = 0;
                if (top < 0) top = 0;
                if (left + width > frame.cols) width = frame.cols - left;
                if (top + height > frame.rows) height = frame.rows - top;
                
                confis.push_back((float)confi);
                rects.push_back(Rect(left, top, width, height));
            }
        }
    }
    
    vector<int> indices;
    NMSBoxes(rects, confis, param.confThreshold, param.nmsThreshold, indices);
    
    return indices.size();
}
