add_executable(test test.c)

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

#libbtbase
if (APPLE)
set(LIBS ${LIBS} "${CMAKE_BINARY_DIR}/src/libbtbase.dylib")
elseif(UNIX)
set(LIBS ${LIBS} "${CMAKE_BINARY_DIR}/src/libbtbase.so")
endif(APPLE)
include_directories("${PROJECT_SOURCE_DIR}/src/")

target_link_libraries(test ${LIBS})
