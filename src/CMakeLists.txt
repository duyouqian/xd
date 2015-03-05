FILE(GLOB_RECURSE CURRENT_HEADERS *.h *.cpp)

SET(BASE_SOURCE mutex.cpp
                )
IF(UNIX)
    MESSAGE("UNIX")
    SET(BASE_SOURCE ${BASE_SOURCE} 
                    mutex_linux.cpp
                    condition_linux.cpp)
ELSEIF(APPLE)
    MESSAGE("APPLE")
ELSEIF(WIN32)
    MESSAGE("WIN32")
ENDIF()

SET(ALL_SOURCE ${CURRENT_HEADERS})
ADD_LIBRARY(xd ${ALL_SOURCE})
TARGET_LINK_LIBRARIES(xd pthread)
