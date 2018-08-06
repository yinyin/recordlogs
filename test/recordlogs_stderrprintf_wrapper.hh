#ifndef TEST_RECORDLOGS_STDERRPRINTF_WRAPPER_HH
#define TEST_RECORDLOGS_STDERRPRINTF_WRAPPER_HH

#include <cstdio>

namespace recordlogs {
namespace test {

class VfprintfWrapper {
  public:
    virtual ~VfprintfWrapper();

    virtual void w_vfprintf(FILE *stream,
                            const char *format,
                            const char *file,
                            int line,
                            const char *fused_text1,
                            int fused_int1);

    virtual void w_vfprintf(FILE *stream,
                            const char *format,
                            const char *file,
                            int line,
                            const char *errortext,
                            int errnum,
                            const char *s1);

    virtual void w_vfprintf(FILE *stream,
                            const char *format,
                            const char *file,
                            int line,
                            const char *errortext,
                            int errnum,
                            const char *s1,
                            int d2);

    virtual void w_vfprintf(FILE *stream,
                            const char *format,
                            const char *file,
                            int line);

    virtual void w_vfprintf(FILE *stream,
                            const char *format,
                            const char *file,
                            int line,
                            const char *s1);

    virtual void w_vfprintf(FILE *stream, const char *format);
};

class FputcWrapper {
  public:
    virtual ~FputcWrapper();

    virtual void w_fputc(char ch, FILE *stream);
};

}  // namespace test
}  // namespace recordlogs

#endif /* TEST_RECORDLOGS_STDERRPRINTF_WRAPPER_HH */
