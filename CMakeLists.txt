cmake_minimum_required(VERSION 3.16)
project(assignment)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(assignment
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        AnomalyDetector.h
        SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h
        timeseries.cpp
        timeseries.h
        MainTrain.cpp)
