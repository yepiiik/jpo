# JPO App with Qt GUI

## Stack

- CMake 3.15 or higher
- vcpkg package manager
- C++17 compatible compiler

## Setup

1. Install vcpkg if you haven't already:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
```

2. Install required packages:
```bash
vcpkg install qt5[essentials] --triplet=x64-windows --binarysource=default
vcpkg install nlohmann-json --triplet=x64-windows --binarysource=default    
vcpkg install curl  --triplet=x64-windows --binarysource=default
```

## Building

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure with CMake:
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

3. Build the project:
```bash
cmake --build .
```