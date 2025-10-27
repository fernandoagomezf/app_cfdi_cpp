cmake_minimum_required(VERSION 4.1.0)

project(CFDI
    VERSION 0.1.0
    DESCRIPTION "Blendwerk CFDI"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS uuid)

add_library(CFDILib STATIC
    src/document.cpp
    src/xmlnode.cpp
    src/xmlreader.cpp
    src/xmlbuffer.cpp
    src/xmlfragmentparser.cpp
    src/xmltextparser.cpp
    src/xmlcommentparser.cpp 
    src/xmlcdataparser.cpp
)
target_include_directories(CFDILib PUBLIC include)
target_link_libraries(CFDILib PRIVATE fmt::fmt)
target_link_libraries(CFDILib PRIVATE Boost::uuid)
target_compile_features(CFDILib PUBLIC cxx_std_23)

add_executable(CFDI
    src/program.cpp
)
target_link_libraries(CFDI PUBLIC CFDILib)
target_link_libraries(CFDI PUBLIC fmt::fmt)
