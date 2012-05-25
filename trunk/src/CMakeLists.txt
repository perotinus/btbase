##############
#Targets
#############
#Static lib
add_library(btbase STATIC libbtbase.c mifare.c)
#Shared lib
add_library(btbase-shared SHARED libbtbase.c mifare.c)
set_target_properties(btbase-shared PROPERTIES OUTPUT_NAME "btbase")
#Util
add_executable(btbaseutil btbaseutil.c mifare.c)


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



########################
#Link with external libs
########################
target_link_libraries(btbaseutil ${LIBS} )
target_link_libraries(btbase-shared ${LIBNFC_LIBRARIES})
