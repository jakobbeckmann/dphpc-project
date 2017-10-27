project(dphpc-project)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/ChanAlgorithm.cpp src/utility.h src/utility.cpp src/Point.h src/constants.h src/main.cpp src/ChanAlgorithm.h src/FileWriter.h src/FileWriter.cpp src/main_test.cpp)
add_executable(dphpc_project ${SOURCE_FILES})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
