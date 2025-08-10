cmake_minimum_required(VERSION 3.16)
project(GrpcQt CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SERVER_SOURCES
  src/server/main.cpp
  src/server//grpc_worker.cpp
  src/server//grpc_worker.h
  src/server/server_window.cpp
  src/server/server_window.h
)

set(CLIENT_SOURCES
  src/client/main.cpp
  src/client/client_window.cpp
  src/client/client_window.h
  src/client/connection_item.cpp
  src/client/connection_item.h
  src/client/connections_model.cpp
  src/client/connections_model.h
  src/client/grpc_stub.cpp
  src/client/grpc_stub.h
  src/client/delegates/button_delegate.cpp
  src/client/delegates/button_delegate.h
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

add_executable(ServerExe ${GUI_TYPE} ${SERVER_SOURCES})
target_link_libraries(ServerExe PRIVATE proto-objects Qt6::Core Qt6::Network Qt6::Widgets)

add_executable(ClientExe ${GUI_TYPE} ${CLIENT_SOURCES})
target_include_directories(ClientExe PRIVATE src/client)
target_link_libraries(ClientExe PRIVATE proto-objects Qt6::Core Qt6::Network Qt6::Widgets)