#ifndef TEST_RECORDLOGS_SYSLOG_WRAPPER_HH
#define TEST_RECORDLOGS_SYSLOG_WRAPPER_HH

namespace recordlogs {
namespace test {

class SyslogWrapper {
  public:
    virtual ~SyslogWrapper();

    virtual void w_vsyslog(int priority,
                           const char *format,
                           const char *file,
                           int line,
                           const char *fused_text1,
                           int fused_int1);

    virtual void w_vsyslog(int priority,
                           const char *format,
                           const char *file,
                           int line,
                           const char *errortext,
                           int errnum,
                           const char *s1);

    virtual void w_vsyslog(int priority,
                           const char *format,
                           const char *file,
                           int line,
                           const char *errortext,
                           int errnum,
                           const char *s1,
                           int d2);

    virtual void w_vsyslog(int priority,
                           const char *format,
                           const char *file,
                           int line);

    virtual void w_vsyslog(int priority,
                           const char *format,
                           const char *file,
                           int line,
                           const char *s1);

    virtual void w_vsyslog(int priority, const char *format);
};

}  // namespace test
}  // namespace recordlogs

#endif /* TEST_RECORDLOGS_SYSLOG_WRAPPER_HH */
