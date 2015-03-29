FILE(GLOB_RECURSE CURRENT_HEADERS *.h *.cpp)

SET(BASE_SOURCE mutex.cpp
                reference_count.cpp
                thread.cpp
                event.cpp
                thread_group.cpp
                log.cpp
                shareable.cpp
                event_loop.cpp
                event_loop_thread.cpp
                event_loop_thread_pool.cpp
                poller.cpp
                channel.cpp
                acceptor.cpp
                buffer.cpp
                tcp_server.cpp
                tcp_connection.cpp)
IF(UNIX)
    MESSAGE("UNIX")
    SET(BASE_SOURCE ${BASE_SOURCE} 
                    mutex_linux.cpp
                    condition_linux.cpp
                    sema_linux.cpp
                    timer_linux.cpp
                    thread_linux.cpp
                    event_linux.cpp
                    file_linux.cpp
                    socket_linux.cpp
                    socketopt_linux.cpp
                    inetaddr_linux.cpp
                    epoll.cpp)
ELSEIF(APPLE)
    MESSAGE("APPLE")
ELSEIF(WIN32)
    MESSAGE("WIN32")
ENDIF()

SET(ALL_SOURCE ${BASE_SOURCE})
#SET(ALL_SOURCE ${CURRENT_HEADERS})
ADD_LIBRARY(xd ${ALL_SOURCE})
TARGET_LINK_LIBRARIES(xd pthread)
