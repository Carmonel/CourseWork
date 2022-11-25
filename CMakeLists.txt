cmake_minimum_required(VERSION 3.16)
project(CourseWork)

set(CMAKE_CXX_STANDARD 23)

add_executable(CourseWork main.cpp ArciverExecutor.h ArciverExecutor.cpp Fano.h Fano.cpp tree.cpp tree.h SyntaxChecker.cpp SyntaxChecker.h Exception.cpp Exception.h Config.h ConfigMapper.cpp ConfigMapper.h Archive.cpp Archive.h Log.cpp Log.h Utils.cpp Utils.h)