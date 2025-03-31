# JPO App with Qt GUI

## Stack

- C++
- Qt5 (for GUI)

## Building tools

- CMake
- vcpkg (packet manager)
- C++17 compiler

> Look below for usage notes

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

The building process is split in two modes: ```debug``` and ```release```.
In debug mode the application will use dynamic linking - that means that additional ```.dll``` files will be attached to ```.exe``` file. On the other hand relase mode will generate only ```.exe``` file after build.

> [!IMPORTANT]  
> Default configuration assumes that project is building for x64-windows platform. . Default generator is set as Visual Studio 2022, therefor make sure Visual Studio 2022 installed.
> In case you want to run build under Linux, MacOS or use different genretor - welcome for changing ```CMakePresets.json```

Build will be stored in ```/build/[build-mode]``` folder

### Debug version (dynamic linking):
```bash
.\build_default_windows.bat
```

---
### Release version (static linking):
```bash
.\build_release_windows.bat
```

---

## VCPKG usage notes

VCPKG packet manager introduces convenient way to install dependencies for C++ projects, however there are some key points that should be considered before using it.

1. ```vcpkg install --info``` - one of the most important list of parameters that should be reviewed before using ```vcpkg install```.
    - By default vcpkg will build each dependency library from scratch - that means it will take some time (for Qt5 approximately an hour) and considerable amount of disk space (for Qt5 approximately 30GB). It is done to ensure integrity with your environment (OS, compilers, etc.). To minimalize disk usage it is often useful to use ```--clean-after-build``` parameter (all raw data will be removed and only built version stored on disk)
2. ```vcpkg.json``` - file for listing necessary dependencies. ```vcpkg install``` will use this as a manifest (the same way ```npm install``` do for ```npm_packages.json``` in JavaScript)