## Repo for AlgoIRL project for CS325

### Authors: Adam Henry & Shane Ludwig

To Compile on Linux: `clang++ src/main.cpp src/Image.cpp src/FeatureDetection.cpp -Wpedantic -DDEBUG_SOBEL -std=c++17 -O2 -o sobel`

To Compile on Windows (maybe): `clang++ src/main.cpp src/Image.cpp src/FeatureDetection.cpp -Wpedantic -DDEBUG_SOBEL -lcomdlg32.lib -std=c++17 -O2 -o sobel`

main.cpp contains the driving code behind the program, take a look!
