set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_FLAGS "-mthumb -fno-builtin -mcpu=cortex-m0 -ffunction-sections \
-fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-math -ftree-vectorize"
CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "-mthumb -fno-builtin -mcpu=cortex-m0 -ffunction-sections \
-fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-mathv -ftree-vectorize"
CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m0 -x assembler-with-cpp"
CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -mthumb -mcpu=cortex-m0 -mabi=aapcs"
CACHE STRING "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS "-mthumb -mcpu=cortex-m0 -mabi=aapcs"
CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS "-mthumb -mcpu=cortex-m0 -mabi=aapcs"
CACHE STRING "" FORCE)
