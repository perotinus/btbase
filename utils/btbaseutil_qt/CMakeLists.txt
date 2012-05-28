#---------------------------
#---------------------------
#Qt detection/setup
#---------------------------
#---------------------------

find_package(Qt4 REQUIRED)


SET(QT_USE_QTGUI TRUE)

#-----------------------
#Set up headers/sources
#-----------------------
set(btbaseutil_qt_SRCS 
    BT_QMainWindow.cpp
    QBEditWidget.cpp
    QHexSpinBox.cpp
    btbaseutil_qt.cpp
    btlibiface.cpp
)

set(btbaseutil_qt_HEADERS
    BT_QMainWindow.h
    QBEditWidget.h
    QHexSpinBox.h
    btlibiface.h
)

SET(btbaseutil_qt_FORMS 
    btbaseutil_qt.ui
    qbeditwidget.ui
)


QT4_WRAP_CPP(btbaseutil_qt_HEADERS_MOC ${btbaseutil_qt_HEADERS})
QT4_WRAP_UI(btbaseutil_qt_HEADERS_FORMS ${btbaseutil_qt_FORMS})

#Need this to make sure that the generated files are found
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

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

#include dir for libbtbase.h
include_directories("${PROJECT_SOURCE_DIR}/src/")

#---------------------------
#QT app executable
#--------------------------

#Create an OSX bundle
if( APPLE )
    set( MACOSX_VERSION "${PROJ_MAJOR}.${PROJ_MINOR}")
    set( MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/utils/btbaseutil_qt/BTBaseUtil.icns" )
    set( MACOSX_BUNDLE_SHORT_VERSION_STRING ${MACOSX_VERSION})
    set( MACOSX_BUNDLE_VERSION ${MACOSX_VERSION})
    set( MACOSX_BUNDLE_LONG_VERSION_STRING "Version ${MACOSX_VERSION}")


  install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"${CMAKE_CURRENT_BINARY_DIR}/BTBaseUtil.app\" \"\" \"\")"
        COMPONENT Runtime)

    add_executable(btbaseutil_qt MACOSX_BUNDLE
        ${btbaseutil_qt_SRCS}
        ${btbaseutil_qt_HEADERS_FORMS}
        ${btbaseutil_qt_HEADERS_MOC} )


    add_custom_command(TARGET btbaseutil_qt POST_BUILD
      #COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/BTBaseUtil.app/Contents/Resources
      COMMAND cp ${MACOSX_BUNDLE_ICON_FILE} ${CMAKE_CURRENT_BINARY_DIR}/BTBaseUtil.app/Contents/Resources
    )

else( NOT APPLE )
    add_executable(btbaseutil_qt 
        ${btbaseutil_qt_SRCS}
        ${btbaseutil_qt_HEADERS_FORMS}
        ${btbaseutil_qt_HEADERS_MOC}
    )
endif( APPLE)


#Rename
set_target_properties(btbaseutil_qt PROPERTIES OUTPUT_NAME "BTBaseUtil")

TARGET_LINK_LIBRARIES(btbaseutil_qt btbase ${QT_LIBRARIES} ${LIBS})
