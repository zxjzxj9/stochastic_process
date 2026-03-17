cmake_minimum_required(VERSION 3.10)
project(StochasticProcessLib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prefer pip-installed pybind11 (Python 3.11 compatible) over the bundled submodule
execute_process(
    COMMAND python -m pybind11 --cmakedir
    OUTPUT_VARIABLE PYBIND11_CMAKE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(PYBIND11_CMAKE_DIR)
    list(APPEND CMAKE_PREFIX_PATH "${PYBIND11_CMAKE_DIR}")
    find_package(pybind11 REQUIRED)
else()
    add_subdirectory(pybind11)
endif()

# -------------------------------------------------------
# Original heat equation solver (chapter3)
# -------------------------------------------------------
pybind11_add_module(heat main.cc)

# -------------------------------------------------------
# Stochastic simulation library (static, no Python dep)
# -------------------------------------------------------
set(STOCHASTIC_SOURCES
    stochastic/process.cpp
    stochastic/payoff.cpp
)

add_library(stochastic_lib STATIC ${STOCHASTIC_SOURCES})
target_include_directories(stochastic_lib PUBLIC stochastic)
target_compile_options(stochastic_lib PRIVATE -Wall -Wextra -O2)
set_property(TARGET stochastic_lib PROPERTY POSITION_INDEPENDENT_CODE ON)

# -------------------------------------------------------
# Python extension module: stochastic
# -------------------------------------------------------
pybind11_add_module(stochastic stochastic/bindings.cpp)
target_link_libraries(stochastic PRIVATE stochastic_lib)
target_include_directories(stochastic PRIVATE stochastic)

# -------------------------------------------------------
# C++ unit tests
# -------------------------------------------------------
add_executable(test_stochastic
    stochastic/tests/test_stochastic.cpp
    stochastic/process.cpp
    stochastic/payoff.cpp
)
target_include_directories(test_stochastic PRIVATE stochastic stochastic/tests)
target_compile_options(test_stochastic PRIVATE -Wall -Wextra -O2)

enable_testing()
add_test(NAME StochasticTests COMMAND test_stochastic)
