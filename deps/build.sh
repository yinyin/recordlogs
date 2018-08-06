#!/bin/bash
# Run with:
# bash deps/build.sh

if [ ! -d deps ]; then
	echo "must in root folder of project"
	exit 1
fi

DEVPROGREQS=("sh" "make" "install")
REQMISSING=0
for PROGREQ in "${DEVPROGREQS[@]}"; do
	PROGNAME=${PROGREQ%% (*}
	PROGPATH=`which "${PROGNAME}"`
	if [ "$?" != "0" ]; then
		echo -e "# cannot reach requirement: \033[1;31m${PROGREQ}\033[0m."
		REQMISSING=1
	else
		PROGVER=`"${PROGNAME}" --version | head -n 1`
		echo -e "- ${PROGREQ}: ${PROGPATH}, \033[1;34m${PROGVER##* }\033[0m."
	fi
done
if [ "0" != "${REQMISSING}" ]; then
	echo "ERR: not all required development tool available."
	exit 1
fi

if [ ! -d build ]; then
	mkdir -p build/{include,lib}
fi

export BASE_FOLDER=`pwd`

echo "going to build Google Test ..."
cd deps/googletest
make -f ../Makefile libgoogletest
cd ../../
