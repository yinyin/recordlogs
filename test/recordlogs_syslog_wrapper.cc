#include "recordlogs_syslog_wrapper.hh"

namespace recordlogs {
namespace test {

SyslogWrapper::~SyslogWrapper() {}

void SyslogWrapper::w_vsyslog(int priority,
                              const char *format,
                              const char *file,
                              int line,
                              const char *fused_text1,
                              int fused_int1) {}

void SyslogWrapper::w_vsyslog(int priority,
                              const char *format,
                              const char *file,
                              int line,
                              const char *errortext,
                              int errnum,
                              const char *s1) {}

void SyslogWrapper::w_vsyslog(int priority,
                              const char *format,
                              const char *file,
                              int line,
                              const char *errortext,
                              int errnum,
                              const char *s1,
                              int d2) {}

void SyslogWrapper::w_vsyslog(int priority,
                              const char *format,
                              const char *file,
                              int line) {}

void SyslogWrapper::w_vsyslog(int priority,
                              const char *format,
                              const char *file,
                              int line,
                              const char *s1) {}

void SyslogWrapper::w_vsyslog(int priority, const char *format) {}
}  // namespace test
}  // namespace recordlogs
