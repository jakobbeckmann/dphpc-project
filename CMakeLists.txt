project(dphpc-project)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/chan2d.cpp src/utility.h src/utility.cpp src/Point.h src/constants.h src/main.cpp src/chan2d.h src/FileWriter.h src/FileWriter.cpp src/main_test.cpp)
add_executable(dphpc_project ${SOURCE_FILES})
