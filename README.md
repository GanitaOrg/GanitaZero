Project: GanitaZero
Group: Ganita
Author: T. Adams, Columbia University grad student

Dependencies:
libbsd, boost 1.55 or later.
Requires GanitaGraph which is sold separately. 
Or, you may obtain it for free: 
git clone git@cloud.ganita.org:ganita/ganitagraph.git

Compile:
cd to root directory containing this README.md file. 
mkdir build
cd build/
cmake -DCMAKE_BUILD_TYPE=Release ../config
make 
sudo make install

Executables:
gzero

Library:
libGanitaZero.a

For further information:
http://cloud.ganita.org:8083 
or email tma2131@columbia.edu. 
