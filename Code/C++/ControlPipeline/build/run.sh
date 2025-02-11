clear; 
cmake -DCMAKE_PREFIX_PATH=/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/libtorch -DCMAKE_OSX_ARCHITECTURES=arm64 ..
cmake --build . --config Release
./ControlPipeline
osascript -e 'display notification "Finished running pipeline" with title "Control Pipeline" sound name "Funk.aiff"'