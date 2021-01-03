cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

include_directories(Basic)
include_directories(StanfordCPPLib)
include_directories(StanfordCPPLib/private)

add_executable(code
        Basic/Basic.cpp Basic/exp.h Basic/exp.cpp
        Basic/program.h Basic/program.cpp
        Basic/parser.cpp Basic/parser.h
        StanfordCPPLib/tokenscanner.h StanfordCPPLib/tokenscanner.cpp
        StanfordCPPLib/simpio.h StanfordCPPLib/simpio.cpp
        StanfordCPPLib/strlib.h StanfordCPPLib/strlib.cpp
        StanfordCPPLib/error.h StanfordCPPLib/error.cpp
        Basic/statement.h Basic/statement.cpp
        Basic/evalstate.h Basic/evalstate.cpp
        #score.cpp
        )
