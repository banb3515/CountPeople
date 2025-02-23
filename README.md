# **Count People**

## **목차**

1. [Count People이란?](#Count-People이란)
2. [시연 영상](#시연-영상)
3. [개발 정보](#개발-정보)
4. [빌드 방법](#빌드-방법)

### **Count People이란?**

-   [2021 Hello New( ) World 해커톤 대회](http://www.hellonewworld.co.kr/bbs/board.php?bo_table=51&wr_id=4) 진행 중 팀원([최정원](https://gitlab.com/BanB3515), [전준혁](https://github.com/zaqwsx2003), 이권찬, 신정근)과 같이 개발한 프로젝트입니다.
-   카메라를 사용하여 해당 위치에 사람이 몇명 모여있는지 서버에 실시간으로 저장하고, 앱에서 지도를 이용하여 사용자가 어느 위치에 몇명이 모여있는지 확인할 수 있습니다.

### **시연 영상**

-   [Camera 시연 영상](./Screenshots/Camera.mp4)
-   [App (Android) 실시간 동기화 시연 영상](./Screenshots/App.mp4)
-   [App (Android) 여러 카메라 지원 시연 영상](./Screenshots/App_MultiCamera.mp4)

### **개발 정보**

#### **카메라 개발 정보**

-   언어 - [C++](https://isocpp.org/) 14
-   라이브러리
    -   [OpenCV 4.5.2](https://opencv.org/releases/)
    -   [YOLOv4](https://github.com/AlexeyAB/darknet)
    -   [cURL](https://curl.se/)

#### **서버 개발 정보**

-   언어 - [JavaScript](https://developer.mozilla.org/ko/docs/Web/JavaScript)
-   라이브러리
    -   [Express](https://expressjs.com/)
    -   [sqlite3](https://github.com/mapbox/node-sqlite3)

#### **앱 개발 정보**

-   언어 - [Kotlin](https://kotlinlang.org/)
-   라이브러리
    -   [Google Maps](https://developers.google.com/maps?hl=ko)

### **빌드 방법**

```bash
git clone https://gitlab.com/BanB3515/CountPeople.git
```

#### 카메라

-   필요 라이브러리 - [라이브러리 설치 (Ubuntu)](https://www.notion.so/banb/Camera-0ca5a2353673492f98e6a7000e1b9831#4cdcbe62e4634116bc68adaee7c24285)
    -   [OpenCV 4.5.2](https://opencv.org/releases/)
    -   [cURL](https://curl.se/)
    -   [CUDA 10.1](https://developer.nvidia.com/cuda-toolkit-archive) (CUDA 사용 시)
    -   [cuDNN 7.6.5 for CUDA 10.1](https://developer.nvidia.com/cudnn) (CUDA 사용 시)

```bash
cd CountPeople/CountPeopleCamera
mkdir build && cd build
# CUDA 미 사용 시, cmake -D CUDA=0 ..
# DEBUG 미 사용 시, cmake -D DEBUG=0 ..
cmake ..
make
./CountPeopleCamera ../cp.ini
```

#### 서버

-   필요 라이브러리
    -   [Express](https://expressjs.com/)
    -   [sqlite3](https://github.com/mapbox/node-sqlite3)

```bash
cd CountPeople/CountPeopleServer
npm install package.json --save
node server.js
```
