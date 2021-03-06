cmake_minimum_required(VERSION 3.4.1)
PROJECT(FFLv2-track)

#  加载公用库 FFLv2
SET(CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR}/../FFLv2-cmakemodule/ )
INCLUDE(${CMAKE_MODULE_PATH}FFLv2lib.cmake)

INCLUDE_DIRECTORIES(${FFLV2_LIB_INCLUDE_PATH})
LINK_DIRECTORIES(${FFLV2_LIB_LIB_PATH})

SET(LOCAL_SRC_FILES
   service/main.cpp;service/trackServer.cpp
   service/trackReader.hpp;service/trackReader.cpp)

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


#  输出路径 bin
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
ADD_EXECUTABLE(FFLv2-track  ${LOCAL_SRC_FILES})
TARGET_LINK_LIBRARIES(FFLv2-track FFL)

add_custom_command(TARGET FFLv2-track POST_BUILD  
  COMMAND  ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/web/track.json ${PROJECT_BINARY_DIR}/../bin/${CMAKE_CFG_INTDIR}/
  COMMAND  ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/web/timeline.html ${PROJECT_BINARY_DIR}/../bin/${CMAKE_CFG_INTDIR}/
 )

