## Repo for AlgoIRL project for CS325

#### Authors: Adam Henry & Shane Ludwig

### Compile the Program for Static Images
To Compile on Linux: `clang++ src/main.cpp src/Image.cpp src/FeatureDetection.cpp -Wpedantic -DDEBUG_SOBEL -std=c++17 -O2 -o sobel`

To Compile on Windows (maybe): `clang++ src/main.cpp src/Image.cpp src/FeatureDetection.cpp -Wpedantic -DDEBUG_SOBEL -lcomdlg32.lib -std=c++17 -O2 -o sobel`
mingw doesn't support c++ 17 and therefore won't compile with this program because we use std::filesystem. (feel free to try and get it to compile without std::fs).

### Compile for the OpenCV Real Time Demo
For the OpenCV Real Time Demo, you must have OpenCV installed with the headers and libraries available where the compiler/linker can see them. The command to compile is listed in the main.cpp file. (BEWARE: ONLY TESTED ON LINUX)

main.cpp contains the driving code behind the program, take a look!
