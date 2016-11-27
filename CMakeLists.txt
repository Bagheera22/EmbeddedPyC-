cmake_minimum_required(VERSION 2.8)
set (CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

SET( PROJ_NAME      "Embedded" )
SET( PROJ_PATH      ${CMAKE_SOURCE_DIR} )
SET( PROJ_OUT_PATH  ${CMAKE_BINARY_DIR} )
SET( PROJ_SOURCES   "")
SET( PROJ_HEADERS   "" )




#set(PYTHON_LIBRARY "C:/Users/AlbaRoger/AppData/Local/Programs/Python/Python35-32/libs")
#set(PYTHON_INCLUDE_DIR "C:/Users/AlbaRoger/AppData/Local/Programs/Python/Python35-32/include")

set(PYTHON_LIBRARY "/Library/Frameworks/Python.framework/Versions/3.5/lib/")
set(PYTHON_INCLUDE_DIR "/Library/Frameworks/Python.framework/Versions/3.5/include/python3.5m/")
 
## include("${PROJ_PATH}/FindPythonLibs.cmake")

project (${PROJ_NAME} )

include_directories(${CMAKE_SOURCE_DIR})

# Default compiler args
if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|.*Clang)")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-g -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL 'MSVC')
# /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
# /FS - Allows multiple cl.exe processes to write to the same .pdb file
# /DEBUG - Enable debug during linking
# /Od - Disables optimization
set(CMAKE_CXX_FLAGS_DEBUG "/Zi /FS /DEBUG /Od /MDd")
# /Ox - Full optimization
set(CMAKE_CXX_FLAGS_RELEASE "/Ox -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ox /Zi /FS /DEBUG")
endif()

# if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-shadow -Wno-padded -Wno-missing-noreturn -Wno-global-constructors")
# endif()

set(OLD_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
if ((MAKE_CXX_COMPILER_ID STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()


LINK_DIRECTORIES()


set(PROJECT_INCLUDE_DIR ${PROJ_PATH}/src)

include_directories(
    ${PROJ_PATH}/src
    )



include_directories("${PYTHON_INCLUDE_DIR}")

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/exe" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/exe" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/exe" )

FILE( GLOB_RECURSE PROJ_SOURCES src/*.cpp )
FILE( GLOB_RECURSE PROJ_HEADERS src/*.h )
set(files ${PROJ_SOURCES} ${PROJ_HEADERS})



macro(GroupSources curdir)
   file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
${PROJECT_SOURCE_DIR}/${curdir}/*)
   foreach(child ${children})
          if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
                  GroupSources(${curdir}/${child})
          else()
                  string(REPLACE "/" "\\" groupname ${curdir})
          # I would like to call the src root folder in a different name, only in visual studio (not mandatory requirement)
                   string(REPLACE "src" "Common" groupname ${groupname})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
          endif()
   endforeach()
endmacro()
# Execute the macro
GroupSources(src)

 add_executable( ${PROJ_NAME} ${files} )

LINK_DIRECTORIES("${PYTHON_LIBRARY}")
#target_link_libraries(${PROJ_NAME} "${PYTHON_LIBRARY}/python35.lib")

target_link_libraries(${PROJ_NAME} "${PYTHON_LIBRARY}/libpython3.5m.dylib")


set_target_properties( ${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/exe"  )
set_target_properties( ${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/exe" )
set_target_properties( ${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/exe"  )
