cmake_minimum_required(VERSION 3.8)
project(dphpc-project)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/ChanAlgorithm.cpp src/utility.h src/utility.cpp src/Point.h src/constants.h src/main.cpp src/ChanAlgorithm.h src/FileWriter.h src/Quickhull.hpp src/Quickhull.cpp
src/JarvisAlgorithm.cpp src/JarvisAlgorithm.h src/GrahamScanAlgorithm.cpp src/GrahamScanAlgorithm.h src/timer.h src/Jarvis_BinJarvis.hpp src/Jarvis_BinJarvis.cpp src/Jarvis_Jarvis.hpp
src/Jarvis_Jarvis.cpp src/Jarvis_Graham.hpp src/Jarvis_Graham.cpp src/Jarvis_Quickhull.hpp src/Jarvis_Quickhull.cpp src/Graham_Jarvis.hpp src/Graham_Jarvis.cpp src/Graham_Graham.hpp src/Graham_Graham.cpp)
add_executable(dphpc_project ${SOURCE_FILES})

if(APPLE)
    set(CMAKE_CXX_COMPILER g++-7)
endif()

if(WINDOWS)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
    endif()


    message(STATUS "Checking OpenMP")
    find_package(OpenMP)
    IF(OPENMP_FOUND)
        message("Found OpenMP! ^_^")
        # add flags for OpenMP
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    ELSE()
        message("Missed OpenMP! x_x")
    ENDIF()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
