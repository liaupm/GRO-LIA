cd ..
make -B
cd debugger
if [ $? -eq 0 ]; then
    echo "CellEngine compiled!"
    echo "Compiling debugger..."
    make -B
    if [ $? -eq 0 ]; then
        echo "Starting debugger..."
        ./bin/linux/debugger $1
    fi
fi
echo " "
echo " "
echo " "
echo " "
echo " "
