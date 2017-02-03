make -C ..
if [ $? -eq 0 ]; then
    cp ../libcellgene.a build
    make MODE=release
    if [ $? -eq 0 ]; then
        build/GeneticTest
    fi
fi