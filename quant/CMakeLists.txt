cmake_minimum_required(VERSION 3.5)

project("Numerical Quant Practice")

add_compile_options("-std=c++11")

add_executable(main.x main.cpp vanilla_option.cpp vanilla_option.h greeks.cpp greeks.h)