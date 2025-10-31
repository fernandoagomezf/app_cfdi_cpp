cmake_minimum_required(VERSION 3.28.0)

project(CFDI
    VERSION 0.1.0
    DESCRIPTION "Blendwerk CFDI"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use dynamic runtime for wxWidgets compatibility
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(wxWidgets_USE_STATIC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(wxWidgets REQUIRED COMPONENTS core base OPTIONAL_COMPONENTS net gl)

include(${wxWidgets_USE_FILE})

add_library(CFDILib STATIC
    # cfdi.util module
    src/cfdi.util.ixx
    # cfdi.xml module
    src/cfdi.xml.ixx
    src/xmlnodetype.ixx
    src/xmlnode.ixx
    src/xmlnode.cpp
    src/xmlbuffer.ixx
    src/xmlbuffer.cpp
    src/xmlfragmentparser.ixx 
    src/xmlfragmentparser.cpp
    src/xmlcdataparser.ixx
    src/xmlcdataparser.cpp
    src/xmlcommentparser.ixx
    src/xmlcommentparser.cpp 
    src/xmldeclarationparser.ixx
    src/xmldeclarationparser.cpp
    src/xmldoctypeparser.ixx
    src/xmldoctypeparser.cpp
    src/xmlelementparser.ixx
    src/xmlelementparser.cpp
    src/xmlprocessinginstructionparser.ixx
    src/xmlprocessinginstructionparser.cpp
    src/xmltextparser.ixx
    src/xmltextparser.cpp
    src/xmlreader.ixx
    src/xmlreader.cpp
    # cfdi.doc module
    src/cfdi.doc.ixx
    src/cfdiheader.ixx
    src/cfdiissuer.ixx
    src/cfdireceiver.ixx
    src/cfdistamp.ixx
    src/cfditransfer.ixx
    src/cfditax.ixx    
    src/cfdiconcept.ixx
    src/cfdicomplement.ixx
    src/cfdisummary.ixx
    src/cfdiwritter.ixx    
    src/cfdiwritter.cpp
    src/cfdidocument.ixx
    src/cfdidocument.cpp
    src/cfdiscanner.ixx
    )
target_include_directories(CFDILib PUBLIC include)
target_compile_features(CFDILib PUBLIC cxx_std_23)

add_executable(CFDI WIN32
    # cfdi.app module
    src/cfdi.app.ixx
    src/window.ixx
    src/window.cpp
    src/application.ixx
    src/application.cpp
)
target_link_libraries(CFDI PUBLIC CFDILib)
target_link_libraries(CFDI PRIVATE ${wxWidgets_LIBRARIES})
target_compile_features(CFDI PUBLIC cxx_std_23)

