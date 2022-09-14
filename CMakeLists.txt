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
        src/page7.cpp
        )
#TODO LZMA SDK DEV
#7z -DNDEBUG  -lpthread -ldl

add_link_options(-s)

target_link_libraries(${PROJECT_NAME} PRIVATE fltk curl nlohmann_json::nlohmann_json lzma
        /home/vuniverse/CLionProjects/mingwInstaller/src/lzma2201/C/sevenz.a)

#in linker option add -s