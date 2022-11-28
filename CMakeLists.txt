cmake_minimum_required(VERSION 3.16)
project(CourseWork)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" CONFIG)
set(BUILD_SHARED_LIBS OFF)

add_executable(CourseWork main.cpp ArchiverExecutor.h ArchiverExecutor.cpp Fano.h Fano.cpp tree.cpp tree.h utils/Exception.cpp utils/Exception.h utils/ConfigMapper.cpp utils/ConfigMapper.h Archive.cpp Archive.h utils/Log.cpp utils/Log.h utils/Utils.cpp utils/Utils.h)