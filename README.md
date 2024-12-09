# gRPC-Qt

## How to Build?

Requirements:
- cmake >= 3.16
- conan >= 2

Some dependencies, such as `protobuf` and `grpc`, are installed via `conan`. That's why you need to specify `-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake"` option, as well as `-DCMAKE_BUILD_TYPE`. But Qt is not installed via conan, since it's extremely huge. 

```bash
cmake -B build -S . -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES="conan_provider.cmake" -DCMAKE_BUILD_TYPE=Release
```

```bash
cmake --build build --config Release
```

# IN PROGRESS