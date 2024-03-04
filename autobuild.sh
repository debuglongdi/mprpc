# !/bin/bash
cd build && cmake .. && make
cd ..
cp -r `pwd`/src/include `pwd`/lib

