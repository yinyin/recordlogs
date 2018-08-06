#ifndef _RECORDLOGS_RECORDLOGS_H_
#define _RECORDLOGS_RECORDLOGS_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <string.h>

void _recordlogs_internal_record_error(int priority, const char *format, ...);

void _recordlogs_internal_record_info(int priority, const char *format, ...);

#define RECORD_ERROR(priority, format, ...)                 \
    do {                                                    \
        int errnum;                                         \
        errnum = errno;                                     \
        _recordlogs_internal_record_error(priority,         \
                                          format,           \
                                          __FILE__,         \
                                          __LINE__,         \
                                          strerror(errnum), \
                                          errnum,           \
                                          ##__VA_ARGS__);   \
    } while (0)

#define RECORD_INFO(priority, format, ...)                            \
    do {                                                              \
        _recordlogs_internal_record_info(                             \
                priority, format, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* _RECORDLOGS_RECORDLOGS_H_ */
