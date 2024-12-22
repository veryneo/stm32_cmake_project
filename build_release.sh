echo "Project building start."
#DIR=build
DIR_BUILD_RELEASE=build/release
if [ -d "$DIR_BUILD_RELEASE" ]; then
    echo "Directory /$DIR_BUILD_RELEASE exist."
    echo "Entry Directory /$DIR_BUILD_RELEASE. "
    cd $DIR_BUILD_RELEASE
else
	echo "Directory /$DIR_BUILD_RELEASE does not exist."
	echo "Make Directory /$DIR_BUILD_RELEASE."
	mkdir -p $DIR_BUILD_RELEASE
	echo "Entry Directory /$DIR_BUILD_RELEASE."
	cd $DIR_BUILD_RELEASE
fi

rm -rf *
cmake ../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../../custom_gcc.cmake

make
