#Generated by VisualGDB (http://visualgdb.com)
#DO NOT EDIT THIS FILE MANUALLY UNLESS YOU ABSOLUTELY NEED TO
#USE VISUALGDB PROJECT PROPERTIES DIALOG INSTEAD

BINARYDIR := Release

#Toolchain
CC := C:/SysGCC/linaro/bin/arm-linux-gnueabihf-gcc.exe
CXX := C:/SysGCC/linaro/bin/arm-linux-gnueabihf-g++.exe
LD := $(CXX)
AR := C:/SysGCC/linaro/bin/arm-linux-gnueabihf-ar.exe
OBJCOPY := C:/SysGCC/linaro/bin/arm-linux-gnueabihf-objcopy.exe

#Additional flags
PREPROCESSOR_MACROS := NDEBUG RELEASE
INCLUDE_DIRS := include src
LIBRARY_DIRS := 
LIBRARY_NAMES := 
ADDITIONAL_LINKER_INPUTS := 
MACOS_FRAMEWORKS := 
LINUX_PACKAGES := 

CFLAGS := -ggdb -ffunction-sections -O3 -fexceptions -march=armv7-a -mtune=cortex-a8 -mfpu=neon -flto-partition=none -finline-limit=900
CXXFLAGS := -ggdb -ffunction-sections -O3 -fexceptions -march=armv7-a -mtune=cortex-a8 -mfpu=neon -flto-partition=none -finline-limit=900
ASFLAGS := 
LDFLAGS := -Wl,-gc-sections  -ggdb -ffunction-sections -O3 -fexceptions -march=armv7-a -mtune=cortex-a8 -mfpu=neon -flto-partition=none -finline-limit=900
COMMONFLAGS := 

START_GROUP := -Wl,--start-group
END_GROUP := -Wl,--end-group

#Additional options detected from testing the toolchain
USE_DEL_TO_CLEAN := 1
IS_LINUX_PROJECT := 1
