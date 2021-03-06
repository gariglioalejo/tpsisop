#!/bin/bash

cd ..
cd ./tpsisoplib
cd ./Debug
gcc -O0 -g3 -Wall -c -fmessage-length=0  -MMD -MF"tpsisoplib.d" -MT"tpsisoplib.d" -o "tpsisoplib.o" "../tpsisoplib.c"
gcc -shared -o "libtpsisoplib.so"  ./tpsisoplib.o   
cd ..
cd ..
cd ./Cpu
cd ./Debug
gcc -I"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"funcionesCpu.d" -MT"funcionesCpu.d" -o "funcionesCpu.o" "../funcionesCpu.c"
gcc -I"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"funcionesNoBloqueantes.d" -MT"funcionesNoBloqueantes.d" -o "funcionesNoBloqueantes.o" "../funcionesNoBloqueantes.c"
gcc -I"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"main.d" -MT"main.d" -o "main.o" "../main.c"
gcc -I"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"parser.d" -MT"parser.d" -o "parser.o" "../parser.c"
gcc -L"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib/Debug" -o "cpu"  ./funcionesCpu.o ./funcionesNoBloqueantes.o ./main.o ./parser.o   -ltpsisoplib -lcommons
./cpu.out ../ArchivoDeConfigCpu
./cpu ../ArchivoDeConfigCpu