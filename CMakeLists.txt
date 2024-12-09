cmake_minimum_required(VERSION 3.16)
project(GrpcQt CXX C)

set(CMAKE_CXX_STANDARD 20)

find_package(Protobuf REQUIRED)

message("VERSION ${protobuf_VERSION_STRING} ${protobuf_INCLUDE_DIR} ${protobuf_LIBRARIES}")

get_cmake_property(IMPORTED_TARGETS IMPORTED_TARGETS)
foreach(target ${IMPORTED_TARGETS})
    message("Found Protobuf target: ${target}")
    if(target MATCHES "protobuf")
        message("Found Protobuf target: ${target}")
    endif()
endforeach()
message("Some message ${IMPORTED_TARGETS}")

add_executable(DummyExe "src/server/main.cpp")
target_include_directories(DummyExe PRIVATE ${protobuf_INCLUDE_DIR})
target_link_libraries(DummyExe Protobuf)
