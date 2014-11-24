rm kernel.out
gcc -L/home/utnso/git/ansisop-panel/Release -o kernel.out  ./kernel.c   -lansisop-panel -lcommons -lpthread
