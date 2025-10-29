cmake_minimum_required(VERSION 4.1.0)

project(CFDI
    VERSION 0.1.0
    DESCRIPTION "Blendwerk CFDI"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Force static linking
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(wxWidgets_USE_STATIC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost REQUIRED COMPONENTS uuid)
find_package(wxWidgets REQUIRED COMPONENTS core base OPTIONAL_COMPONENTS net gl)

include(${wxWidgets_USE_FILE})

add_library(CFDILib STATIC
    src/document.cpp
    src/summary.cpp
    src/xmlnode.cpp
    src/xmlreader.cpp
    src/xmlbuffer.cpp
    src/xmlfragmentparser.cpp
    src/xmltextparser.cpp
    src/xmlcommentparser.cpp 
    src/xmlcdataparser.cpp
    src/xmlprocessinginstructionparser.cpp
    src/xmldeclarationparser.cpp
    src/xmldoctypeparser.cpp
    src/xmlelementparser.cpp
    src/directoryscanner.cpp
    src/utilities.cpp
    src/output.cpp
)
target_include_directories(CFDILib PUBLIC include)
target_link_libraries(CFDILib PRIVATE Boost::uuid)
target_compile_features(CFDILib PUBLIC cxx_std_23)

add_executable(CFDI WIN32
    src/application.cpp
    src/window.cpp
)
target_link_libraries(CFDI PUBLIC CFDILib)
target_link_libraries(CFDI PRIVATE ${wxWidgets_LIBRARIES})
target_compile_features(CFDI PUBLIC cxx_std_23)

