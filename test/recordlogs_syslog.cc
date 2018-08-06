#include <cstdarg>
#include <cstdint>

#include <syslog.h>
#include <unistd.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "recordlogs/recordlogs.h"

#include "helper.hh"
#include "recordlogs_syslog_wrapper.hh"

using ::testing::_;
using ::testing::InSequence;
using ::testing::StrEq;

namespace recordlogs {
namespace test {

using helper::caculate_format_sequence_code;

class MockSyslogWrapper : public SyslogWrapper {
  public:
    MockSyslogWrapper();
    virtual ~MockSyslogWrapper();

    MOCK_METHOD6(w_vsyslog,
                 void(int priority,
                      const char *format,
                      const char *file,
                      int line,
                      const char *fused_text1,
                      int fused_int1));
    MOCK_METHOD7(w_vsyslog,
                 void(int priority,
                      const char *format,
                      const char *file,
                      int line,
                      const char *errortext,
                      int errnum,
                      const char *s1));
    MOCK_METHOD8(w_vsyslog,
                 void(int priority,
                      const char *format,
                      const char *file,
                      int line,
                      const char *errortext,
                      int errnum,
                      const char *s1,
                      int d2));
    MOCK_METHOD4(
            w_vsyslog,
            void(int priority, const char *format, const char *file, int line));
    MOCK_METHOD5(w_vsyslog,
                 void(int priority,
                      const char *format,
                      const char *file,
                      int line,
                      const char *s1));
    MOCK_METHOD2(w_vsyslog, void(int priority, const char *format));
};

static MockSyslogWrapper *syslogWrapper = NULL;

MockSyslogWrapper::MockSyslogWrapper() : SyslogWrapper() {
    if (NULL != syslogWrapper) {
        ADD_FAILURE() << "syslog wrapper instance already existed";
    } else {
        syslogWrapper = this;
    }
}

MockSyslogWrapper::~MockSyslogWrapper() {
    if (this != syslogWrapper) {
        ADD_FAILURE() << "syslog wrapper instance not same one";
    } else {
        syslogWrapper = NULL;
    }
}

static void proxy_leading_vsyslog(int32_t seqcode,
                                  int priority,
                                  const char *format,
                                  va_list ap) {
    const char *file = va_arg(ap, char *);
    int line = va_arg(ap, int);
    switch (seqcode) {
        case 785: {
            const char *errortext = va_arg(ap, char *);
            int errnum = va_arg(ap, int);
            syslogWrapper->w_vsyslog(
                    priority, format, file, line, errortext, errnum);
        } break;
        case 49: {
            syslogWrapper->w_vsyslog(priority, format, file, line);
        } break;
        default:
            ADD_FAILURE() << "unknown sequence code (part: file, line, "
                             "errtext, errnum): "
                          << seqcode;
    }
}

static void proxy_followup_vsyslog(int32_t seqcode,
                                   int priority,
                                   const char *format,
                                   va_list ap) {
    switch (seqcode) {
        case 3:
            syslogWrapper->w_vsyslog(priority, format);
            break;
        default:
            ADD_FAILURE() << "unknown sequence code (part: general): "
                          << seqcode;
    }
}

extern "C" void mocked_vsyslog(int priority, const char *format, va_list ap) {
    if (NULL == syslogWrapper) {
        ADD_FAILURE() << "require syslog wrapper";
        return;
    }
    int32_t seqcode = caculate_format_sequence_code(format);
    switch (seqcode) {
        case 785:
        case 49:
            proxy_leading_vsyslog(seqcode, priority, format, ap);
            break;
        case 3:
            proxy_followup_vsyslog(seqcode, priority, format, ap);
            break;
        default:
            ADD_FAILURE() << "unknown sequence code (dispatch): " << seqcode
                          << " - " << format;
    }
}

static const char *NOT_EXISTED_PATH = "/tmp/not-existed/201802-31415925";

#define LOGMSG_ERROR_PREFIX "[%s:%d] %s (errno=%d)"
#define LOGMSG_ERROR_LEADING LOGMSG_ERROR_PREFIX ": "
#define LOGMSG_ERROR_LARGE LOGMSG_ERROR_PREFIX " ..."

#define LOGMSG_INFO_PREFIX "[%s:%d]"
#define LOGMSG_INFO_LEADING LOGMSG_INFO_PREFIX ": "
#define LOGMSG_INFO_LARGE LOGMSG_INFO_PREFIX " ..."

#define MESSAGE_TEXT_BASE "cannot delete given path"
#define MESSAGE_TEXT_LONGBASE                                                  \
    "cannot delete given path; padding, padding, padding, padding, padding, "  \
    "padding, padding, padding, padding, padding, padding, padding, padding, " \
    "padding, padding, padding, padding, padding, padding, padding, padding, " \
    "padding, padding, padding, padding, padding, padding, padding, padding, " \
    "padding, padding, padding, padding, padding, padding, padding, padding"

TEST(RecordErrorTest, NoArgsNormal) {
    static const char *message_text = MESSAGE_TEXT_BASE;
    static const char *format_text = LOGMSG_ERROR_LEADING MESSAGE_TEXT_BASE;
    MockSyslogWrapper mockSyslogWrapper;
    int retcode;
    EXPECT_CALL(mockSyslogWrapper,
                w_vsyslog(LOG_INFO, StrEq(format_text), __FILE__, _, _, _));
    if (0 != (retcode = unlink(NOT_EXISTED_PATH))) {
        RECORD_ERROR(LOG_INFO, message_text);
    } else {
        ADD_FAILURE() << "unlink results normal return code: " << retcode;
    }
}

TEST(RecordErrorTest, NoArgsLong) {
    static const char *message_text = MESSAGE_TEXT_LONGBASE;
    static const char *format_text_1 = LOGMSG_ERROR_LARGE;
    static const char *format_text_2 = MESSAGE_TEXT_LONGBASE;
    MockSyslogWrapper mockSyslogWrapper;
    int retcode;
    InSequence s0;
    EXPECT_CALL(mockSyslogWrapper,
                w_vsyslog(LOG_INFO, StrEq(format_text_1), __FILE__, _, _, _));
    EXPECT_CALL(mockSyslogWrapper, w_vsyslog(LOG_INFO, StrEq(format_text_2)));
    if (0 != (retcode = unlink(NOT_EXISTED_PATH))) {
        RECORD_ERROR(LOG_INFO, message_text);
    } else {
        ADD_FAILURE() << "unlink results normal return code: " << retcode;
    }
}

TEST(RecordInfoTest, NoArgsNormal) {
    static const char *message_text = MESSAGE_TEXT_BASE;
    static const char *format_text = LOGMSG_INFO_LEADING MESSAGE_TEXT_BASE;
    MockSyslogWrapper mockSyslogWrapper;
    int retcode;
    EXPECT_CALL(mockSyslogWrapper,
                w_vsyslog(LOG_INFO, StrEq(format_text), __FILE__, _));
    if (0 != (retcode = unlink(NOT_EXISTED_PATH))) {
        RECORD_INFO(LOG_INFO, message_text);
    } else {
        ADD_FAILURE() << "unlink results normal return code: " << retcode;
    }
}

TEST(RecordInfoTest, NoArgsLong) {
    static const char *message_text = MESSAGE_TEXT_LONGBASE;
    static const char *format_text_1 = LOGMSG_INFO_LARGE;
    static const char *format_text_2 = MESSAGE_TEXT_LONGBASE;
    MockSyslogWrapper mockSyslogWrapper;
    int retcode;
    InSequence s0;
    EXPECT_CALL(mockSyslogWrapper,
                w_vsyslog(LOG_INFO, StrEq(format_text_1), __FILE__, _));
    EXPECT_CALL(mockSyslogWrapper, w_vsyslog(LOG_INFO, StrEq(format_text_2)));
    if (0 != (retcode = unlink(NOT_EXISTED_PATH))) {
        RECORD_INFO(LOG_INFO, message_text);
    } else {
        ADD_FAILURE() << "unlink results normal return code: " << retcode;
    }
}

}  // namespace test
}  // namespace recordlogs
