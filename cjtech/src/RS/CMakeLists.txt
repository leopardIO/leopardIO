AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(RS ${DIR_SRCS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -O2 ")
target_link_libraries(nodeserver boost_system boost_thread mysqlclient) 

