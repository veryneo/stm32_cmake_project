echo "Project building start."
#DIR=build
DIR_BUILD_DEBUG=build/debug
if [ -d "$DIR_BUILD_DEBUG" ]; then
    echo "Directory /$DIR_BUILD_DEBUG exist."
    echo "Entry Directory /$DIR_BUILD_DEBUG. "
    cd $DIR_BUILD_DEBUG
else
	echo "Directory /$DIR_BUILD_DEBUG does not exist."
	echo "Make Directory /$DIR_BUILD_DEBUG."
	mkdir -p $DIR_BUILD_DEBUG
	echo "Entry Directory /$DIR_BUILD_DEBUG."
	cd $DIR_BUILD_DEBUG
fi

rm -rf *
cmake ../.. -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../custom_gcc.cmake

ninja
