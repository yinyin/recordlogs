#include <cstdarg>
#include <cstdint>
#include <cstdio>

#include <syslog.h>
#include <unistd.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "recordlogs/recordlogs.h"

#include "helper.hh"
#include "recordlogs_stderrprintf_wrapper.hh"

using ::testing::_;
using ::testing::InSequence;
using ::testing::StrEq;

namespace recordlogs {
namespace test {

using helper::caculate_format_sequence_code;

class MockVfprintfWrapper : public VfprintfWrapper {
  public:
    MockVfprintfWrapper();
    virtual ~MockVfprintfWrapper();

    MOCK_METHOD6(w_vfprintf,
                 void(FILE *stream,
                      const char *format,
                      const char *file,
                      int line,
                      const char *fused_text1,
                      int fused_int1));
    MOCK_METHOD7(w_vfprintf,
                 void(FILE *stream,
                      const char *format,
                      const char *file,
                      int line,
                      const char *errortext,
                      int errnum,
                      const char *s1));
    MOCK_METHOD8(w_vfprintf,
                 void(FILE *stream,
                      const char *format,
                      const char *file,
                      int line,
                      const char *errortext,
                      int errnum,
                      const char *s1,
                      int d2));
    MOCK_METHOD4(
            w_vfprintf,
            void(FILE *stream, const char *format, const char *file, int line));
    MOCK_METHOD5(w_vfprintf,
                 void(FILE *stream,
                      const char *format,
                      const char *file,
                      int line,
                      const char *s1));
    MOCK_METHOD2(w_vfprintf, void(FILE *stream, const char *format));
};

static MockVfprintfWrapper *vfprintfWrapper = NULL;

MockVfprintfWrapper::MockVfprintfWrapper() : VfprintfWrapper() {
    if (NULL != vfprintfWrapper) {
        ADD_FAILURE() << "vfprintf wrapper instance already existed";
    } else {
        vfprintfWrapper = this;
    }
}

MockVfprintfWrapper::~MockVfprintfWrapper() {
    if (this != vfprintfWrapper) {
        ADD_FAILURE() << "vfprintf wrapper instance not same one";
    } else {
        vfprintfWrapper = NULL;
    }
}

class MockFputcWrapper : public FputcWrapper {
  public:
    MockFputcWrapper();
    virtual ~MockFputcWrapper();

    MOCK_METHOD2(w_fputc, void(char ch, FILE *stream));
};

static MockFputcWrapper *fputcWrapper = NULL;

MockFputcWrapper::MockFputcWrapper() : FputcWrapper() {
    if (NULL != fputcWrapper) {
        ADD_FAILURE() << "fputc wrapper instance already existed";
    } else {
        fputcWrapper = this;
    }
}

MockFputcWrapper::~MockFputcWrapper() {
    if (this != fputcWrapper) {
        ADD_FAILURE() << "fputc wrapper instance not same one";
    } else {
        fputcWrapper = NULL;
    }
}

static void proxy_leading_vfprintf(int32_t seqcode,
                                   FILE *stream,
                                   const char *format,
                                   va_list ap) {
    const char *file = va_arg(ap, char *);
    int line = va_arg(ap, int);
    switch (seqcode) {
        case 785: {
            const char *errortext = va_arg(ap, char *);
            int errnum = va_arg(ap, int);
            vfprintfWrapper->w_vfprintf(
                    stream, format, file, line, errortext, errnum);
        } break;
        case 49: {
            vfprintfWrapper->w_vfprintf(stream, format, file, line);
        } break;
        default:
            ADD_FAILURE() << "unknown sequence code (part: file, line, "
                             "errtext, errnum): "
                          << seqcode;
    }
}

static void proxy_followup_vfprintf(int32_t seqcode,
                                    FILE *stream,
                                    const char *format,
                                    va_list ap) {
    switch (seqcode) {
        case 3:
            vfprintfWrapper->w_vfprintf(stream, format);
            break;
        default:
            ADD_FAILURE() << "unknown sequence code (part: general): "
                          << seqcode;
    }
}

extern "C" void mocked_vfprintf(FILE *stream, const char *format, va_list ap) {
    if (NULL == vfprintfWrapper) {
        ADD_FAILURE() << "require vfprintf wrapper";
        return;
    }
    int32_t seqcode = caculate_format_sequence_code(format);
    switch (seqcode) {
        case 785:
        case 49:
            proxy_leading_vfprintf(seqcode, stream, format, ap);
            break;
        case 3:
            proxy_followup_vfprintf(seqcode, stream, format, ap);
            break;
        default:
            ADD_FAILURE() << "unknown sequence code (dispatch): " << seqcode
                          << " - " << format;
    }
}

extern "C" void mocked_fputc(char ch, FILE *stream) {
    if (NULL == fputcWrapper) {
        ADD_FAILURE() << "require fputc wrapper";
        return;
    }
    fputcWrapper->w_fputc(ch, stream);
}

static const char *NOT_EXISTED_PATH = "/tmp/not-existed/201802-31415925";

#define LOGMSG_PREFIX_HEADING "["

#define LOGMSG_ERROR_LEADING ">%s:%d] %s (errno=%d): "
#define LOGMSG_INFO_LEADING ">%s:%d]: "

#define MESSAGE_TEXT_BASE "cannot delete given path"
#define MESSAGE_TEXT_LONGBASE                                                  \
    "cannot delete given path; padding, padding, padding, padding, padding, "  \
    "padding, padding, padding, padding, padding, padding, padding, padding, " \
    "padding, padding, padding, padding, padding, padding, padding, padding, " \
    "padding, padding, padding, padding, padding, padding, padding, padding, " \
    "padding, padding, padding, padding, padding, padding, padding, padding"

TEST(StderrPrintfRecordErrorTest, NoArgsNormal) {
    static const char *message_text = MESSAGE_TEXT_BASE;
    static const char *format_text = LOGMSG_PREFIX_HEADING
            "INFO" LOGMSG_ERROR_LEADING MESSAGE_TEXT_BASE "\n";
    MockVfprintfWrapper mockVfprintfWrapper;
    int retcode;
    EXPECT_CALL(mockVfprintfWrapper,
                w_vfprintf(stderr, StrEq(format_text), __FILE__, _, _, _));
    if (0 != (retcode = unlink(NOT_EXISTED_PATH))) {
        RECORD_ERROR(LOG_INFO, message_text);
    } else {
        ADD_FAILURE() << "unlink results normal return code: " << retcode;
    }
}

TEST(StderrPrintfRecordErrorTest, NoArgsLong) {
    static const char *message_text = MESSAGE_TEXT_LONGBASE;
    static const char *format_text_1 =
            LOGMSG_PREFIX_HEADING "INFO" LOGMSG_ERROR_LEADING;
    static const char *format_text_2 = MESSAGE_TEXT_LONGBASE;
    MockVfprintfWrapper mockVfprintfWrapper;
    MockFputcWrapper mockFputcWrapper;
    int retcode;
    InSequence s0;
    EXPECT_CALL(mockVfprintfWrapper,
                w_vfprintf(stderr, StrEq(format_text_1), __FILE__, _, _, _));
    EXPECT_CALL(mockVfprintfWrapper, w_vfprintf(stderr, StrEq(format_text_2)));
    EXPECT_CALL(mockFputcWrapper, w_fputc('\n', stderr));
    if (0 != (retcode = unlink(NOT_EXISTED_PATH))) {
        RECORD_ERROR(LOG_INFO, message_text);
    } else {
        ADD_FAILURE() << "unlink results normal return code: " << retcode;
    }
}

TEST(StderrPrintfRecordInfoTest, NoArgsNormal) {
    static const char *message_text = MESSAGE_TEXT_BASE;
    static const char *format_text = LOGMSG_PREFIX_HEADING
            "INFO" LOGMSG_INFO_LEADING MESSAGE_TEXT_BASE "\n";
    MockVfprintfWrapper mockVfprintfWrapper;
    int retcode;
    EXPECT_CALL(mockVfprintfWrapper,
                w_vfprintf(stderr, StrEq(format_text), __FILE__, _));
    if (0 != (retcode = unlink(NOT_EXISTED_PATH))) {
        RECORD_INFO(LOG_INFO, message_text);
    } else {
        ADD_FAILURE() << "unlink results normal return code: " << retcode;
    }
}

TEST(StderrPrintfRecordInfoTest, NoArgsLong) {
    static const char *message_text = MESSAGE_TEXT_LONGBASE;
    static const char *format_text_1 =
            LOGMSG_PREFIX_HEADING "INFO" LOGMSG_INFO_LEADING;
    static const char *format_text_2 = MESSAGE_TEXT_LONGBASE;
    MockVfprintfWrapper mockVfprintfWrapper;
    MockFputcWrapper mockFputcWrapper;
    int retcode;
    InSequence s0;
    EXPECT_CALL(mockVfprintfWrapper,
                w_vfprintf(stderr, StrEq(format_text_1), __FILE__, _));
    EXPECT_CALL(mockVfprintfWrapper, w_vfprintf(stderr, StrEq(format_text_2)));
    EXPECT_CALL(mockFputcWrapper, w_fputc('\n', stderr));
    if (0 != (retcode = unlink(NOT_EXISTED_PATH))) {
        RECORD_INFO(LOG_INFO, message_text);
    } else {
        ADD_FAILURE() << "unlink results normal return code: " << retcode;
    }
}

}  // namespace test
}  // namespace recordlogs
