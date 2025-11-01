cmake_minimum_required(VERSION 4.1.0)

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
    src/cfdi.util/cfdi.util.ixx
    # cfdi.xml module
    src/cfdi.xml/cfdi.xml.ixx
    src/cfdi.xml/xmlnodetype.ixx
    src/cfdi.xml/xmlnode.ixx
    src/cfdi.xml/xmlnode.cpp
    src/cfdi.xml/xmlbuffer.ixx
    src/cfdi.xml/xmlbuffer.cpp
    src/cfdi.xml/xmlfragmentparser.ixx 
    src/cfdi.xml/xmlfragmentparser.cpp
    src/cfdi.xml/xmlcdataparser.ixx
    src/cfdi.xml/xmlcdataparser.cpp
    src/cfdi.xml/xmlcommentparser.ixx
    src/cfdi.xml/xmlcommentparser.cpp 
    src/cfdi.xml/xmldeclarationparser.ixx
    src/cfdi.xml/xmldeclarationparser.cpp
    src/cfdi.xml/xmldoctypeparser.ixx
    src/cfdi.xml/xmldoctypeparser.cpp
    src/cfdi.xml/xmlelementparser.ixx
    src/cfdi.xml/xmlelementparser.cpp
    src/cfdi.xml/xmlprocessinginstructionparser.ixx
    src/cfdi.xml/xmlprocessinginstructionparser.cpp
    src/cfdi.xml/xmltextparser.ixx
    src/cfdi.xml/xmltextparser.cpp
    src/cfdi.xml/xmlreader.ixx
    src/cfdi.xml/xmlreader.cpp
    # cfdi.doc module
    src/cfdi.doc/cfdi.doc.ixx
    src/cfdi.doc/cfdiheader.ixx
    src/cfdi.doc/cfdiissuer.ixx
    src/cfdi.doc/cfdireceiver.ixx
    src/cfdi.doc/cfdistamp.ixx
    src/cfdi.doc/cfditransfer.ixx
    src/cfdi.doc/cfditax.ixx    
    src/cfdi.doc/cfdiconcept.ixx
    src/cfdi.doc/cfdicomplement.ixx
    src/cfdi.doc/cfdisummary.ixx
    src/cfdi.doc/cfdiwritter.ixx    
    src/cfdi.doc/cfdiwritter.cpp
    src/cfdi.doc/cfdidocument.ixx
    src/cfdi.doc/cfdidocument.cpp
    src/cfdi.doc/cfdiscanner.ixx
    src/cfdi.doc/cfdiscanner.cpp
    )
target_include_directories(CFDILib PUBLIC include)
target_compile_features(CFDILib PUBLIC cxx_std_23)

add_executable(CFDI WIN32
    # cfdi.app module
    src/cfdi.app/cfdi.app.ixx
    src/cfdi.app/window.ixx
    src/cfdi.app/window.cpp
    src/cfdi.app/application.ixx
    src/cfdi.app/application.cpp
)
target_link_libraries(CFDI PUBLIC CFDILib)
target_link_libraries(CFDI PRIVATE ${wxWidgets_LIBRARIES})
target_compile_features(CFDI PUBLIC cxx_std_23)

