#!/bin/sh
OriginDir=`pwd`
cd `dirname $0`

TestProject=cpputest
Repository=https://github.com/cpputest/cpputest

git 2>&1 >/dev/null
if test 1 -ne $?; then
	echo "Error: git isn't installed."
fi

if test -d .git; then
	echo "Directory \".git\" is already exists. skip.."
else
	echo "-- Inits git directory..."
	git init
fi

if test -d "ext/${TestProject}"; then
	echo "Directory \"ext/${TestProject}\" is already exists. skip.."
else
	echo "-- adds submodule ${TestProject}..."
	git submodule add ${Repository} ext/${TestProject} --depth=1
fi

echo "-- Starts test build..."
cd build
cmake .. && make
RCode=$?
if test 0 -ne ${RCode}; then
	echo "!! Error detected in builds. (Error code: ${RCode})!!"
	exit 1
fi


echo "Done."
cd "${OriginDir}"
