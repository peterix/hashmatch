cmake_minimum_required(VERSION 3.1)
project(hashmatch CXX)

add_executable(hashmatch main.cpp)
set_property(TARGET hashmatch PROPERTY CXX_STANDARD 11)

install(TARGETS hashmatch RUNTIME DESTINATION bin)
