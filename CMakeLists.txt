cmake_minimum_required(VERSION 4.0)
project(wordle)

set(CMAKE_CXX_STANDARD 20)

add_executable(wordle main.cpp
        WordleDict.cpp
        WordleDict.h
        consts.h
        WordleGame.cpp
        WordleGame.h)

find_package(CURL REQUIRED)
target_link_libraries(wordle PRIVATE CURL::libcurl)