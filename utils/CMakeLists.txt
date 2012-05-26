add_subdirectory(btbaseutil_qt)

add_executable(test test.c)
add_executable(btbaseutil btbaseutil.c ../src/mifare.c)

####################
#Grab external libs
####################
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

#Include directory for libbtbase.h
include_directories("${PROJECT_SOURCE_DIR}/src/")

if (APPLE)
    target_link_libraries(test ${LIBS} btbase)
else (NOT APPLE)    
    target_link_libraries(test ${LIBS} btbase)
endif (APPLE)

target_link_libraries(btbaseutil ${LIBS} )
