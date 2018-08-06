
CFLAGS      += -Wall -Iinclude/
CXXFLAGS    += -Wall -Iinclude/

TESTCXXFLAGS := $(CXXFLAGS) $(TESTCXXFLAGS)     \
		-Ideps/googletest/googletest/include/   \
		-Ideps/googletest/googlemock/include/

TESTLDFLAGS := $(LDFLAGS) $(TESTLDFLAGS)

TESTLIBS := $(LIBS) build/lib/gmock_main.a -lpthread

test/testhelper: test/testhelper.cc test/helper.cc
	$(CXX) $(TESTCXXFLAGS) -o $@ $^ $(TESTLDFLAGS) $(TESTLIBS)

test/recordlogs_syslog_with_mock.o: src/recordlogs_syslog.c
	$(CC) $(CFLAGS) -DMOCKED_VSYSLOG=mocked_vsyslog -c -o $@ $<

test/recordlogs_syslog: test/recordlogs_syslog_with_mock.o test/helper.cc test/recordlogs_syslog_wrapper.cc test/recordlogs_syslog.cc
	$(CXX) $(TESTCXXFLAGS) -o $@ $^ $(TESTLDFLAGS) $(TESTLIBS)

test: test/testhelper test/recordlogs_syslog
	test/testhelper
	test/recordlogs_syslog

clean-test-binary:
	$(RM) test/testhelper
	$(RM) test/recordlogs_syslog test/recordlogs_syslog_with_mock.o

clean-test-coverage:
	$(RM) *.gcov
	$(RM) test/*.gcda test/*.gcno
	$(RM) src/*.gcda src/*.gcno
	$(RM) *.gcda *.gcno

clean-test: clean-test-binary clean-test-coverage

clean: clean-test
	$(RM) src/*.o
