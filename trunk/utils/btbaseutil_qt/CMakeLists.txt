#---------------------------
#---------------------------
#Qt detection/setup
#---------------------------
#---------------------------

find_package(Qt4 REQUIRED)

#-----------------------
#Set up headers/sources
#-----------------------
set(btbaseutil_qt_SRCS 
    BT_QWidget.cpp
    btbaseutil_qt.cpp
    btlibiface.cpp
)

set(btbaseutil_qt_HEADERS
    BT_QWidget.h
    btlibiface.h
)

QT4_WRAP_CPP(btbaseutil_qt_HEADERS_MOC ${btbaseutil_qt_HEADERS})

#-------------------
#Add includes and such
#----------------------
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})


#---------------------------
#---------------------------
#Grab other external libs
#---------------------------
#---------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(LIBUSB REQUIRED)
include_directories(${LIBUSB_INCLUDE_DIRS})
set(LIBS ${LIBS} ${LIBUSB_LIBRARIES})

find_package(PCSC REQUIRED)
include_directories(${PCSC_INCLUDE_DIRS})
set(LIBS ${LIBS} ${PCSC_LIBRARIES})

find_package(LIBNFC 1.6.0 REQUIRED)
include_directories(${LIBNFC_INCLUDE_DIRS})
set(LIBS ${LIBS} ${LIBNFC_LIBRARIES})

#libbtbase
if (APPLE)
set(LIBS ${LIBS} "${CMAKE_BINARY_DIR}/src/libbtbase.a")
elseif(UNIX)
set(LIBS ${LIBS} "${CMAKE_BINARY_DIR}/src/libbtbase.a")
endif(APPLE)
include_directories("${PROJECT_SOURCE_DIR}/src/")

#---------------------------
#QT app executable
#---------------------------
add_executable(btbaseutil_qt 
    ${btbaseutil_qt_SRCS}
    ${btbaseutil_qt_HEADERS_MOC}
)

#Rename
set_target_properties(btbaseutil_qt PROPERTIES OUTPUT_NAME "BTBaseUtil")

TARGET_LINK_LIBRARIES(btbaseutil_qt ${QT_LIBRARIES} ${LIBS})
