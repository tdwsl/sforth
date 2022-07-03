cc -O2 -Wall -Isource -c source/io.c source/machine.c source/sforth.c
cc -O2 -Wall -Isource *.o source/interpreter.c -o sforth
ar ruv libsforth.a *.o
ranlib libsforth.a
