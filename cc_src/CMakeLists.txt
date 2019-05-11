PROJECT(SimpleHeatPDESolver)
cmake_minimum_required(VERSION 3.10)

add_subdirectory(pybind11)

#pybind11_add_module(SimpleHeatPDESolver main.cc)
pybind11_add_module(heat main.cc)
