//
//  main.cpp
//  CountPeopleCamera Main
//
//  Created by Printf("HelloWorld!") on 2021/07/29.
//

#include "Detector.hpp"
#include "Sender.hpp"
#include "iniparser.hpp"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
    /* 명령 인수 확인
        ./HeyMask help
        ./HeyMask [INI 파일 경로]
    */
    if ((argc == 2 && !strcmp(argv[1], "help")) || argc != 2)
    {
        cout << "# 사용법: " << argv[0] << " [INI 파일 경로]\n";
        
        if (argc == 2 && !strcmp(argv[1], "help")) return 0;
        else return -1;
    }
    
    // 설정 파일 불러오기
    string iniFile = argv[1];
    INI::File ini;
    if (!ini.Load(iniFile))
    {
        cout << "# " << iniFile << " 파일을 찾을 수 없습니다.\n";
        return -1;
    }
    
    INI::Section* configSec = ini.GetSection("Config");
    INI::Section* detectorSec = ini.GetSection("Detector");
    INI::Section* senderSec = ini.GetSection("Sender");
    
    // 전역 설정
    string source = configSec->GetValue("source", "").AsString();
    if (source == "")
    {
        cout << "# [Config] source 값이 잘못되었습니다.\n";
        return -1;
    }
    string locName = configSec->GetValue("loc-name", "").AsString();
    double loc1 = configSec->GetValue("loc1", "").AsDouble();
    double loc2 = configSec->GetValue("loc2", "").AsDouble();
    if (locName == "")
    {
        cout << "# [Config] loc-name 값이 잘못되었습니다.\n";
        return -1;
    }
    int width = configSec->GetValue("width", 0).AsInt();
    if (width <= 0)
    {
        cout << "# [Config] width 값이 잘못되었습니다.\n";
        return -1;
    }
    int height = configSec->GetValue("height", 0).AsInt();
    if (height <= 0)
    {
        cout << "# [Config] height 값이 잘못되었습니다.\n";
        return -1;
    }
    
    // Detector 설정
    string gpuStr = detectorSec->GetValue("gpu", "").AsString();
    if (gpuStr == "")
    {
        cout << "# [Detector] gpu 값이 잘못되었습니다.\n";
        return -1;
    }
    bool useGPU;
    istringstream(gpuStr.c_str()) >> useGPU;
    
    DetectorParam detectorParam =
    {
        detectorSec->GetValue("data", "").AsString(),
        (float)detectorSec->GetValue("confidence-threshold", 0.0).AsDouble(),
        (float)detectorSec->GetValue("nms-threshold", 0.0).AsDouble(),
        detectorSec->GetValue("width", 0).AsInt(),
        detectorSec->GetValue("height", 0).AsInt(),
        useGPU
    };
    
    // Sender 설정
    SenderParam senderParam =
    {
        senderSec->GetValue("server", "").AsString()
    };
    
    ini.Unload();
    
    // 소스 초기화
    VideoCapture capture;
    // 카메라 사용
    if (atoi(source.c_str()) != 0 || source.compare("0") == 0)
    {
        capture.open(stoi(source.c_str()));
        if (!capture.isOpened())
        {
            cout << "# " << source << "번 카메라를 찾을 수 없습니다.\n";
            return -1;
        }
        
        capture.set(CAP_PROP_FRAME_WIDTH, width); // 해상도 너비 설정
        capture.set(CAP_PROP_FRAME_HEIGHT, height); // 해상도 높이 설정
    }
    // 비디오 파일 또는 URL 사용
    else
    {
        capture.open(source);
        if (!capture.isOpened())
        {
            // URL 형식이 아니면 비디오 파일로 인식
            if (source.find("://") == string::npos)
                cout << "# " << source << " 파일을 찾을 수 없습니다.\n";
            else
                cout << "# " << source << "에 연결할 수 없습니다.\n";
            return -1;
        }
    }
    
    // 이전 사람 수
    size_t cntPeople = 0;
    
    Detector* detector = new Detector(detectorParam);
    Sender* sender = new Sender(senderParam);
    
    // 프레임
    Mat frame;
    while (true)
    {
        capture >> frame;
        
        if (frame.empty()) break;
        
        size_t cnt = detector->Detect(frame);
        
        if (cnt != cntPeople)
        {
            cntPeople = cnt;
            
            string data;
            data = "locName=";
            data.append(locName);
            data.append("&");
            data.append("loc1=");
            data.append(to_string(loc1));
            data.append("&");
            data.append("loc2=");
            data.append(to_string(loc2));
            data.append("&");
            data.append("count=");
            data.append(to_string(cnt));
            
            sender->Send(data);
        }
        
#if DEBUG == 1
        imshow("Debug", frame);
        
        if (waitKey(10) == 27) break;
#endif
    }
    
    return 0;
}
