# cd /Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/SeafloorSetup/build
clear; 
cmake -DCMAKE_PREFIX_PATH=/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/libtorch -DCMAKE_OSX_ARCHITECTURES=arm64 ..
cmake --build . --config Release
./TransmitterSetup