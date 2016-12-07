cd ..; make; cd test

clang -o create create_test.c -I../include/ -L../build/ -lcellsignal -lm
clang -o getcells getcells.c -I../include/ -L../build/ -lcellsignal -lm
clang -o uptake uptake.c -I../include/ -L../build/ -lcellsignal -lm
clang -o step step.c -I../include/ -L../build/ -lcellsignal -lm
