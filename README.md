# JPO App with Qt GUI

## Stack

- C++
- Qt6 (for GUI)

## Building tools

- CMake
- vcpkg (packet manager)
- C++17 compiler

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