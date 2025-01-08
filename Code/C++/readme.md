# setup
follow the same steps as that of https://pytorch.org/cppdocs/installing.html


- when running the cmake, run the following 

```bash
cmake -DCMAKE_PREFIX_PATH=/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/libtorch -DCMAKE_OSX_ARCHITECTURES=arm64 ..
cmake --build . --config Release
```

- if you run into the macos malicious software issue, just go the directory above libtorch and run the following 

```bash
sudo xattr -r -d com.apple.quarantine /Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/libtorch/lib/*.dylib
```