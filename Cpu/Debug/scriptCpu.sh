#!/bin/bash


gcc -I"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"funcionesCpu.d" -MT"funcionesCpu.d" -o "funcionesCpu.o" "../funcionesCpu.c"
gcc -I"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"funcionesNoBloqueantes.d" -MT"funcionesNoBloqueantes.d" -o "funcionesNoBloqueantes.o" "../funcionesNoBloqueantes.c"
gcc -I"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"main.d" -MT"main.d" -o "main.o" "../main.c"
gcc -I"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"parser.d" -MT"parser.d" -o "parser.o" "../parser.c"
gcc -L"/home/utnso/git/tp-2014-2c-nmi/tpsisoplib/Debug" -o "cpu"  ./funcionesCpu.o ./funcionesNoBloqueantes.o ./main.o ./parser.o   -ltpsisoplib -lcommons
