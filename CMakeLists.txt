cmake_minimum_required(VERSION 3.23)

project(mingwInstaller)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(mingwInstaller
        src/main.cpp
        src/unpack.c
        src/DownloadButton.cpp
        src/Manager.cpp
        src/page.cpp
        src/page0.cpp
        src/page1.cpp
        src/page2.cpp
        src/page3.cpp
        src/page4.cpp
        src/page5.cpp
        src/page6.cpp

        src/lzma2201/C/Bcj2.c
        src/lzma2201/C/Bra.c
        src/lzma2201/C/Bra86.c
        src/lzma2201/C/BraIA64.c
        src/lzma2201/C/BraIA64.c
        src/lzma2201/C/CpuArch.c
        src/lzma2201/C/Delta.c
        src/lzma2201/C/Lzma2Dec.c
        src/lzma2201/C/LzmaDec.c
        src/lzma2201/C/Ppmd7.c
        src/lzma2201/C/Ppmd7Dec.c
        src/lzma2201/C/7zCrc.c
        src/lzma2201/C/7zCrcOpt.c
        src/lzma2201/C/Sha256.c
        src/lzma2201/C/Sha256Opt.c
        src/lzma2201/C/7zAlloc.c
        src/lzma2201/C/7zArcIn.c
        src/lzma2201/C/7zBuf.c
        src/lzma2201/C/7zBuf2.c
        src/lzma2201/C/7zDec.c
        src/lzma2201/C/7zFile.c
        src/lzma2201/C/7zStream.c
        src/logo.cpp src/link.cpp src/icon.cpp)
#7z -DNDEBUG  -lpthread -ldl

add_link_options(-s)

target_link_libraries(${PROJECT_NAME} PRIVATE fltk curl fltk_images) #nlohmann_json::nlohmann_json )
       # ${PROJECT_SOURCE_DIR}/src/lzma2201/C/sevenz.a)

#in linker option add -s