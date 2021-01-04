cmake_minimum_required(VERSION 3.5.1)
project(AutoPiMaster CXX)

#aasdk
add_subdirectory(aasdk)
set(AASDK_INCLUDE_DIRS ${aasdk_SOURCE_DIR}/include)
set(AASDK_LIBRARIES ${aasdk_SOURCE_DIR}/lib/libaasdk.so)
set(AASDK_PROTO_INCLUDE_DIRS ${aasdk_BINARY_DIR})
set(AASDK_PROTO_LIBRARIES ${aasdk_SOURCE_DIR}/lib/libaasdk_proto.so)
message("Hi")
# open Auto
add_subdirectory(openauto)
set(OPENAUTO_INCLUDE_DIRS ${openauto_SOURCE_DIR}/include)

# Auto Pi
add_subdirectory(autopi)
set(AUTOPI_INCLUDE_DIRS ${autopi_SOURCE_DIR}/include)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(base_directory ${CMAKE_CURRENT_SOURCE_DIR})
#set(resources_directory ${base_directory}/assets)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${base_directory})
set(EXECUTABLE_OUTPUT_PATH ${base_directory}/bin)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_definitions(-DBOOST_ALL_DYN_LINK)

find_package(Boost REQUIRED COMPONENTS system log OPTIONAL_COMPONENTS unit_test_framework)
find_package(libusb-1.0 REQUIRED)
find_package(Qt5 COMPONENTS Multimedia MultimediaWidgets Bluetooth)
find_package(KF5BluezQt REQUIRED)
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(rtaudio REQUIRED)

if(RPI_BUILD)
    add_definitions(-DUSE_OMX -DOMX_SKIP64BIT)
    set(BCM_HOST_LIBRARIES "/opt/vc/lib/libbcm_host.so")
    set(BCM_HOST_INCLUDE_DIRS "/opt/vc/include")
    set(ILCLIENT_INCLUDE_DIRS "/opt/vc/src/hello_pi/libs/ilclient")
    set(ILCLIENT_LIBRARIES "/opt/vc/src/hello_pi/libs/ilclient/libilclient.a;/opt/vc/lib/libvcos.so;/opt/vc/lib/libvcilcs.a;/opt/vc/lib/libvchiq_arm.so")
endif(RPI_BUILD)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                   ${Qt5Multimedia_INCLUDE_DIRS}
                   ${Qt5MultimediaWidgets_INCLUDE_DIRS}
                   ${Qt5Widgets_INCLUDE_DIRS}
                   ${Qt5Bluetooth_INCLUDE_DIRS}
                   ${Boost_INCLUDE_DIRS}
                   ${LIBUSB_1_INCLUDE_DIRS}
                   ${PROTOBUF_INCLUDE_DIRS}
                   ${OPENSSL_INCLUDE_DIRS}
                   ${RTAUDIO_INCLUDE_DIRS}
                   ${AASDK_PROTO_INCLUDE_DIRS}
                   ${AASDK_INCLUDE_DIRS}
                   ${OPENAUTO_INCLUDE_DIRS}
                   ${BCM_HOST_INCLUDE_DIRS}
                   ${ILCLIENT_INCLUDE_DIRS}
                   ${include_directory})
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) 

file(GLOB_RECURSE source_files ${sources})
