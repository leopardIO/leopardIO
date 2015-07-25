AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(rootserver ${DIR_SRCS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -O2 ")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(rootserver boost_system boost_thread)

