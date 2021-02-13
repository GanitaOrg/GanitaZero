Project: GanitaZero
Group: Ganita
Author: T. Adams, Columbia University grad student

Dependencies:
libbsd; on Ubuntu, try sudo apt-get install libbsd-dev.
Requires GanitaGraph which is sold separately. 
Or, you may obtain it for free in gitlab.

Compile:
cd to root directory containing this README.md file. 
mkdir build
cd build/
cmake -DCMAKE_BUILD_TYPE=Release ../config
make 
sudo make install

Executables:
gzero, gztgf

Library:
libGanitaZero.a

For further information, email tma2131@columbia.edu. 
