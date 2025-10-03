cd /Users/vrsreeganesh/Documents/Github/AUV/Code/C++/build
# cmake -DFFTW3_ROOT=/opt/homebrew/opt/fftw ..
cmake -DFFTW3_ROOT=/usr/local/Cellar/fftw ..
# cmake ..
echo "\n\n ======================== finished cmake ========================\n\n"
make 
echo "> finished make. Beginning run"
./auv
