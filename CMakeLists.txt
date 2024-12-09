cmake_minimum_required(VERSION 3.16)
project(GrpcQt CXX C)

set(CMAKE_CXX_STANDARD 20)

find_package(absl REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(DummyExe "src/server/main.cpp")
target_include_directories(DummyExe PRIVATE ${protobuf_INCLUDE_DIR} ${absl_INCLUDE_DIR})
target_link_libraries(DummyExe protobuf::protobuf abseil::abseil)
