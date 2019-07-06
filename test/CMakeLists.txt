cmake_minimum_required(VERSION 3.13)
project(tests C ASM)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

set(SRC_DIR ../src)

include_directories(${SRC_DIR})

add_executable(nco_test
    ${SRC_DIR}/nco.c
    src/test_nco.c
)

add_executable(ac_synth_test
    ${SRC_DIR}/nco.c
    ${SRC_DIR}/ac_synth.c
    src/test_ac_synth.c
)
