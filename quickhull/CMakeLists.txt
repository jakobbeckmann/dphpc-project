project(dphpc-project-quickhull)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp ../src/Point.h src/Quickhull.hpp src/Quickhull.cpp)
add_executable(quickhull ${SOURCE_FILES})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
endif()
