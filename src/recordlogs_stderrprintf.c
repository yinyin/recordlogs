#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

#define FORMAT_BUFFER_SIZE 128

static const char *LOGMSG_PREFIX_HEADING = "[";

static const char *LOGMSG_ERROR_LEADING = ">%s:%d] %s (errno=%d): ";
static const char *LOGMSG_INFO_LEADING = ">%s:%d]: ";

#if 0
static const char *LOGMSG_SUFFIX_LEADING = ": ";
static const char *LOGMSG_SUFFIX_LARGE = "...";

#define LOGMSG_ERROR_PREFIX ">%s:%d] %s (errno=%d)"
static const char *LOGMSG_ERROR_LEADING = LOGMSG_ERROR_PREFIX ": ";
static const char *LOGMSG_ERROR_LARGE = LOGMSG_ERROR_PREFIX " ...";

#define LOGMSG_INFO_PREFIX ">%s:%d]"
static const char *LOGMSG_INFO_LEADING = LOGMSG_INFO_PREFIX ": ";
static const char *LOGMSG_INFO_LARGE = LOGMSG_INFO_PREFIX " ...";
#endif

#ifdef MOCKED_VFPRINTF
#warning "with mocked vfprintf"
#define vfprintf MOCKED_VFPRINTF
void MOCKED_VFPRINTF(FILE *stream, const char *format, va_list ap);
#endif /* MOCKED_VFPRINTF */

#ifdef MOCKED_FPUTC
#warning "with mocked fputc"
#define fputc MOCKED_FPUTC
void MOCKED_FPUTC(char ch, FILE *stream);
#endif /* MOCKED_FPUTC */

/* Longest priority text: EMERGENCY */
#define MAX_PRIORITY_TEXT_LEN 10

static const char *_recordlogs_internal_get_logging_priority_text(
        int priority) {
    /* make sure MAX_PRIORITY_TEXT_LEN is updated with this function. */
    switch (priority) {
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_INFO:
            return "INFO";
        case LOG_NOTICE:
            return "NOTICE";
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERR:
            return "ERROR";
        case LOG_CRIT:
            return "CRITICAL";
        case LOG_ALERT:
            return "ALERT";
        case LOG_EMERG:
            return "EMERGENCY";
    }
    return "UNKNOWN";
}

static void _recordlogs_internal_record_impl(size_t msg_leading_len,
                                             const char *msg_leading,
                                             int priority,
                                             const char *format,
                                             va_list ap) {
    char fmtbuf[FORMAT_BUFFER_SIZE];
    size_t fmtbuf_remain;
    char *fmtbuf_p;
    const char *priority_text =
            _recordlogs_internal_get_logging_priority_text(priority);
    fmtbuf_remain = FORMAT_BUFFER_SIZE - 1;
    fmtbuf_p = stpncpy(fmtbuf, LOGMSG_PREFIX_HEADING, fmtbuf_remain);
    fmtbuf_remain -= strlen(LOGMSG_PREFIX_HEADING);
    fmtbuf_p = stpncpy(fmtbuf_p, priority_text, fmtbuf_remain);
    fmtbuf_remain -= strlen(priority_text);
    fmtbuf_p = stpncpy(fmtbuf_p, msg_leading, fmtbuf_remain);
    fmtbuf_remain -= msg_leading_len;
    fmtbuf_remain -= 2;
    if (strlen(format) > fmtbuf_remain) {
        vfprintf(stderr, fmtbuf, ap);
        vfprintf(stderr, format, ap);
        fputc('\n', stderr);
    } else {
        fmtbuf_p = stpncpy(fmtbuf_p, format, fmtbuf_remain);
        *fmtbuf_p = '\n';
        fmtbuf_p++;
        *fmtbuf_p = '\0';
        vfprintf(stderr, fmtbuf, ap);
    }
}

void _recordlogs_internal_record_error(int priority, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    _recordlogs_internal_record_impl(strlen(LOGMSG_ERROR_LEADING),
                                     LOGMSG_ERROR_LEADING,
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
                                     priority,
                                     format,
                                     ap);
    va_end(ap);
}
