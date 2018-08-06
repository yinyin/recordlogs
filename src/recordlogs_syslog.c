#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>

#define FORMAT_BUFFER_SIZE 128

#define LOGMSG_ERROR_PREFIX "[%s:%d] %s (errno=%d)"
static const char *LOGMSG_ERROR_LEADING = LOGMSG_ERROR_PREFIX ": ";
static const char *LOGMSG_ERROR_LARGE = LOGMSG_ERROR_PREFIX " ...";

#define LOGMSG_INFO_PREFIX "[%s:%d]"
static const char *LOGMSG_INFO_LEADING = LOGMSG_INFO_PREFIX ": ";
static const char *LOGMSG_INFO_LARGE = LOGMSG_INFO_PREFIX " ...";

#ifdef MOCKED_VSYSLOG
#warning "with mocked vsyslog"
#define vsyslog MOCKED_VSYSLOG
void MOCKED_VSYSLOG(int priority, const char *format, va_list ap);
#endif /* MOCKED_VSYSLOG */

static void _recordlogs_internal_record_impl(size_t msg_leading_len,
                                             const char *msg_leading,
                                             const char *msg_large,
                                             int priority,
                                             const char *format,
                                             va_list ap) {
    size_t fmtbuf_remain;
    fmtbuf_remain = FORMAT_BUFFER_SIZE - msg_leading_len - 1;
    if (strlen(format) > fmtbuf_remain) {
        vsyslog(priority, msg_large, ap);
        vsyslog(priority, format, ap);
    } else {
        char fmtbuf[FORMAT_BUFFER_SIZE];
        strncpy(fmtbuf, msg_leading, FORMAT_BUFFER_SIZE);
        strncpy(fmtbuf + msg_leading_len, format, fmtbuf_remain);
        fmtbuf[(FORMAT_BUFFER_SIZE - 1)] = '\0';
        vsyslog(priority, fmtbuf, ap);
    }
}

void _recordlogs_internal_record_error(int priority, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    _recordlogs_internal_record_impl(strlen(LOGMSG_ERROR_LEADING),
                                     LOGMSG_ERROR_LEADING,
                                     LOGMSG_ERROR_LARGE,
                                     priority,
                                     format,
                                     ap);
    va_end(ap);
}

void _recordlogs_internal_record_info(int priority, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    _recordlogs_internal_record_impl(strlen(LOGMSG_INFO_LEADING),
                                     LOGMSG_INFO_LEADING,
                                     LOGMSG_INFO_LARGE,
                                     priority,
                                     format,
                                     ap);
    va_end(ap);
}
