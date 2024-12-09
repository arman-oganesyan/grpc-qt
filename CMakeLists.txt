cmake_minimum_required(VERSION 3.16)
project(GrpcQt CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SERVER_SOURCES
  src/server/main.cpp
  src/server/server_window.cpp
)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Network Widgets)

# build protocol
add_subdirectory(src/proto)

if(WIN32)
  set(GUI_TYPE WIN32)
elseif(APPLE)
  set(GUI_TYPE MACOSX_BUNDLE)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(DummyExe ${GUI_TYPE} ${SERVER_SOURCES})
target_link_libraries(DummyExe protocol Qt6::Core Qt6::Network Qt6::Widgets)
