cmake_minimum_required (VERSION 2.8.11)
project (DolwinPlayground)

# Choose an X86/X64 build

if (CMAKE_C_SIZEOF_DATA_PTR EQUAL 8)
  set(X86 OFF)
  set(X64 ON)
else ()
  set(X86 ON)
  set(X64 OFF)
endif ()

# Previously, the HLE subsystem required a base address of 0x400000 for the VMCall instruction. VMCall is now disabled and will be redone.

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Ttext-segment=400000")

add_definitions (-D_LINUX)
add_definitions (-DCMAKE_BUILD_TYPE=Debug)

# ../GekkoCore/Gekko.h:196:66: warning: `fastcall` attribute ignored [-Wattributes]
add_compile_options(-Wno-attributes)

# ThirdParty
add_subdirectory (ThirdParty/fmt/Scripts)

# Backends
add_subdirectory (SRC/Backends/AudioNull/Scripts)
add_subdirectory (SRC/Backends/VideoNull/Scripts)
add_subdirectory (SRC/Backends/GraphicsNull/Scripts)
add_subdirectory (SRC/Backends/PadNull/Scripts)

# DolwinEmu Core
add_subdirectory (SRC/Common/Scripts)
add_subdirectory (SRC/Debugger/Scripts)
add_subdirectory (SRC/DolwinEmu/Scripts)
add_subdirectory (SRC/DSP/Scripts)
add_subdirectory (SRC/DVD/Scripts)
add_subdirectory (SRC/GekkoCore/Scripts)
add_subdirectory (SRC/GX/Scripts)
add_subdirectory (SRC/Hardware/Scripts)
add_subdirectory (SRC/HighLevel/Scripts)

# UI
add_subdirectory (SRC/UI/Playground/Scripts)
