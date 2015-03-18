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

ADD_EXECUTABLE(ts_atomics ts_atomics.cpp)
TARGET_LINK_LIBRARIES(ts_atomics xd)

ADD_EXECUTABLE(ts_sema ts_sema.cpp)
TARGET_LINK_LIBRARIES(ts_sema xd)

ADD_EXECUTABLE(ts_timer ts_timer.cpp)
TARGET_LINK_LIBRARIES(ts_timer xd)

ADD_EXECUTABLE(ts_thread ts_thread.cpp)
TARGET_LINK_LIBRARIES(ts_thread xd)

ADD_EXECUTABLE(ts_group ts_group.cpp)
TARGET_LINK_LIBRARIES(ts_group xd)

ADD_EXECUTABLE(ts_file ts_file.cpp)
TARGET_LINK_LIBRARIES(ts_file xd)

ADD_EXECUTABLE(ts_log_new ts_log_new.cpp)
TARGET_LINK_LIBRARIES(ts_log_new xd)

ADD_EXECUTABLE(ts_blockqueue ts_blockqueue.cpp)
TARGET_LINK_LIBRARIES(ts_blockqueue xd)

ADD_EXECUTABLE(ts_shareptr ts_shareptr.cpp)
TARGET_LINK_LIBRARIES(ts_shareptr xd)

ADD_EXECUTABLE(ts_ioevent ts_ioevent.cpp)
TARGET_LINK_LIBRARIES(ts_ioevent xd)

ADD_EXECUTABLE(ts_inetaddr ts_inetaddr.cpp)
TARGET_LINK_LIBRARIES(ts_inetaddr xd)

ADD_EXECUTABLE(ts_socket ts_socket.cpp)
TARGET_LINK_LIBRARIES(ts_socket xd)

