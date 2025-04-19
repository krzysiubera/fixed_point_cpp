#~/bin/bash

BUILD_DIR=build

function configure {
    echo "Configuring..."
    cmake -S . -B $BUILD_DIR
}

function build {
    echo "Building..."
    cmake --build $BUILD_DIR
}

function run {
    echo "Running..."
    $BUILD_DIR/fixed-point-cpp
}

function clean {
    echo "Cleaning..."
    rm -rf $BUILD_DIR
}

function rebuild {
    clean
    configure
    build
}

if [ "$1" == "run" ]; then
    configure && build && run
elif [ "$1" == "clean" ]; then
    clean
elif [ "$1" == "rebuild" ]; then
    rebuild
else
    configure && build
fi