# JPO project

## Stack:

- Qt6 (version 6.9.0)

## Requirements:

```
Qt6::Quick  
Qt6::Network
Qt6::Concurrent
Qt6::Positioning     // Additional package
Qt6::Charts          // Additional package
```

## Build instruction:

1. Select ```Build for Run configuration``` in Qt creator ```Build``` tab MinGW compile. That will create ```build/Desktop_Qt_6_9_0_MinGW_64_bit-Release/``` folder
2. Create folder ```windows-deploy``` inside of release folder 
3. Copy ```appjpo.exe``` to ```windows-deploy```
4. In terminal run ```windeployqt6.exe appjpo.exe```