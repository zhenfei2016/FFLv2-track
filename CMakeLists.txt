cmake_minimum_required(VERSION 3.4.1)

PROJECT(FFLv2-track)

INCLUDE_DIRECTORIES(../FFLv2-lib/include)
LINK_DIRECTORIES(../install/FFLv2-lib/libs)

SET(LOCAL_SRC_FILES
   main.cpp;trackMain.cpp)

if (MSVC)
    add_definitions("/wd4819")
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif(MSVC)



ADD_EXECUTABLE(FFLv2-track  ${LOCAL_SRC_FILES})
TARGET_LINK_LIBRARIES(FFLv2-track FFL)

