project(proto_files)

find_package(absl REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTO_SRCS
    api.proto
)

protobuf_generate_cpp(PROTO_CPPS PROTO_HDRS ${PROTO_SRCS})
set(PROTO_OUTPUT ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

add_library(protocol STATIC ${PROTO_CPPS})
target_include_directories(protocol PUBLIC ${protobuf_INCLUDE_DIR} ${absl_INCLUDE_DIR})
target_link_libraries(protocol PRIVATE protobuf::protobuf abseil::abseil)
