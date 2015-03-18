FILE(GLOB_RECURSE CURRENT_HEADERS *.h *.cpp)

SET(BASE_SOURCE mutex.cpp
                reference_count.cpp
                thread.cpp
                event.cpp
                thread_group.cpp
                log.cpp
                shareable.cpp
                event_loop.cpp)
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
                    inetaddr_linux.cpp)
ELSEIF(APPLE)
    MESSAGE("APPLE")
ELSEIF(WIN32)
    MESSAGE("WIN32")
ENDIF()

SET(ALL_SOURCE ${BASE_SOURCE})
ADD_LIBRARY(xd ${ALL_SOURCE})
TARGET_LINK_LIBRARIES(xd pthread)
