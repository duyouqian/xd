# 添加头文件
INCLUDE_DIRECTORIES(../src/)

ADD_EXECUTABLE(ts_main ts_main.cpp)
TARGET_LINK_LIBRARIES(ts_main xd)

ADD_EXECUTABLE(ts_mutex ts_mutex.cpp)
TARGET_LINK_LIBRARIES(ts_mutex xd)

