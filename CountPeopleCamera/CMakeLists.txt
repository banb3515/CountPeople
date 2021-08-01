cmake_minimum_required(VERSION 3.1)

# ----- Project -----
project(
  CountPeopleCamera
  VERSION 1.0
  LANGUAGES CXX
)

# DEBUG 여부
add_definitions(-D DEBUG=1) # 1: 사용, 0: 미사용

# CUDA 사용 여부
add_definitions(-D CUDA=1) # 1: 사용, 0: 미사용

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES ./CountPeopleCamera/main.cpp ./CountPeopleCamera/Detector.cpp ./CountPeopleCamera/Sender.cpp)
# ----- Project End -----

# ----- Thread -----
find_package(Threads REQUIRED)
# ----- Thread End -----

# ----- OpenCV -----
set(OpenCV_DIR [OpenCV.cmake PATH])
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# ----- OpenCV End -----

# ----- cURL -----
set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
# ----- OpenCV End -----

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Threads::Threads ${OpenCV_LIBS} ${CURL_LIBRARIES})
