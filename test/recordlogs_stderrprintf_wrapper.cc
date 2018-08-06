#include "recordlogs_stderrprintf_wrapper.hh"

namespace recordlogs {
namespace test {

VfprintfWrapper::~VfprintfWrapper() {}

void VfprintfWrapper::w_vfprintf(FILE *stream,
                                 const char *format,
                                 const char *file,
                                 int line,
                                 const char *fused_text1,
                                 int fused_int1) {}

void VfprintfWrapper::w_vfprintf(FILE *stream,
                                 const char *format,
                                 const char *file,
                                 int line,
                                 const char *errortext,
                                 int errnum,
                                 const char *s1) {}

void VfprintfWrapper::w_vfprintf(FILE *stream,
                                 const char *format,
                                 const char *file,
                                 int line,
                                 const char *errortext,
                                 int errnum,
                                 const char *s1,
                                 int d2) {}

void VfprintfWrapper::w_vfprintf(FILE *stream,
                                 const char *format,
                                 const char *file,
                                 int line) {}

void VfprintfWrapper::w_vfprintf(FILE *stream,
                                 const char *format,
                                 const char *file,
                                 int line,
                                 const char *s1) {}

void VfprintfWrapper::w_vfprintf(FILE *stream, const char *format) {}

FputcWrapper::~FputcWrapper() {}

void FputcWrapper::w_fputc(char ch, FILE *stream) {}
}  // namespace test
}  // namespace recordlogs
