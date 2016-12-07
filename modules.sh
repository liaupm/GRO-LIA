cd CellEngine
make MODE=$1 -B
cd ..
cd cellsignal
cd cellsignal-0.0.3
make MODE=$1 -B
cd ..
cd ..
cd cellsignals
make MODE=$1 -B

