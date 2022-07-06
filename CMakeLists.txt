cmake_minimum_required(VERSION 3.18)

project(OPM_recognizer)

add_executable(recognizer grammar.cpp tableLR.cpp tableLRt.cpp opm.cpp opm_recognizer.cpp main.cpp)
