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
.\bootstrap-vcpkg.bat
```
2. In ```CMakePresets.json``` change ```[path_to_vcpkg]``` to actual path of installed vcpkg in previous step
```
"cacheVariables": {
    "CMAKE_TOOLCHAIN_FILE": "[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake"
}
```


## Building

- Run build script for debug version:
```bash
.\build_default_windows.bat
```
or
- Run build script for release version::
```bash
.\build_release_windows.bat
```