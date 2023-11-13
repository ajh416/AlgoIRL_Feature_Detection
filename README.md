## Repo for AlgoIRL project for CS325

#### Authors: Adam Henry & Shane Ludwig

### Compile the Program for Static Images
To Compile on Linux: `clang++ src/main.cpp src/Image.cpp src/FeatureDetection.cpp -Wpedantic -DDEBUG_SOBEL -std=c++17 -O2 -o sobel`

To Compile on Windows: Use Visual Studio! Open the solution and run it using F5.\
To use the command line: `clang++ src/main.cpp src/Image.cpp src/FeatureDetection.cpp -Wpedantic -DDEBUG_SOBEL -l comdlg32.lib -O2 -o sobel`\
Using MinGW is untested, it may or may not work.

### Compile for the OpenCV Real Time Demo
For the OpenCV Real Time Demo, you must have OpenCV installed with the headers and libraries available where the compiler/linker can see them. The command to compile is listed in the main.cpp file. (BEWARE: ONLY TESTED ON LINUX)

main.cpp contains the driving code whereas the actual Sobel operator and convolution are contained in FeatureDetection.cpp, take a look!
