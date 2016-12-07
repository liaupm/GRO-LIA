cd CellEngine
mingw32-make all MODE=%1
cd ..
cd cellsignal\cellsignal-0.0.3
mingw32-make all MODE=%1
cd ..
cd ..
cd cellsignals
mingw32-make all MODE=%1
cd ..
