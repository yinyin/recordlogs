
all:
	echo 'build with "bash deps/build.sh" instead'

LIBGOOGLETEST_GTEST_TARGET = gtest.a gtest_main.a
LIBGOOGLETEST_GMOCK_TARGET = gmock.a gmock_main.a

libgoogletest:
	cd googletest/make \
		&& make $(LIBGOOGLETEST_GTEST_TARGET) \
		&& install $(LIBGOOGLETEST_GTEST_TARGET) ../../../../build/lib
	cd googlemock/make \
		&& make $(LIBGOOGLETEST_GMOCK_TARGET) \
		&& install $(LIBGOOGLETEST_GMOCK_TARGET) ../../../../build/lib
	cd googletest/make && make clean
	cd googlemock/make && make clean
