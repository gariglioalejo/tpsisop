#!/bin/sh

git clone https://github.com/sisoputnfrba/so-commons-library.git
sudo apt-get install libcunit1-dev
cd so-commons-library
sudo make install