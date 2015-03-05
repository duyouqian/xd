# 添加头文件
INCLUDE_DIRECTORIES(../src/)

ADD_EXECUTABLE(ts_main ts_main.cpp)
TARGET_LINK_LIBRARIES(ts_main xd)

ADD_EXECUTABLE(ts_log ts_log.cpp)
TARGET_LINK_LIBRARIES(ts_log xd)

ADD_EXECUTABLE(ts_mutex ts_mutex.cpp)
TARGET_LINK_LIBRARIES(ts_mutex xd)

ADD_EXECUTABLE(ts_cond ts_condition.cpp)
TARGET_LINK_LIBRARIES(ts_cond xd)

ADD_EXECUTABLE(ts_extends ts_extends.cpp)
TARGET_LINK_LIBRARIES(ts_extends xd)

ADD_EXECUTABLE(ts_atomics ts_atomics.cpp)
TARGET_LINK_LIBRARIES(ts_atomics xd)

