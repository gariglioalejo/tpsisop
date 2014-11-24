gcc -O0 -g3 -Wall -c -fmessage-length=0  -MMD -MF"tpsisoplib.d" -MT"tpsisoplib.d" -o "tpsisoplib.o" "../tpsisoplib.c"
gcc -shared -o "libtpsisoplib.so"  ./tpsisoplib.o
cp ./libtpsisoplib.so /lib