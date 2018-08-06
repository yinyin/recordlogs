#include <cstdint>

#include <gtest/gtest.h>

namespace recordlogs {
namespace test {
namespace helper {

int32_t get_format_character_code(char c) {
    switch (c) {
        case 's':
            return 0;
        case 'd':
            return 1;
        case '%':
            return -2;
    }
    return -1;
}

int32_t caculate_format_sequence_code(const char *format) {
    static const int SEQ_STEP = 4;
    int32_t result = 3;
    const char *p = format;
    bool fmtflag = false;
    while ('\0' != *p) {
        char c = *p;
        if (fmtflag) {
            int32_t digit = get_format_character_code(c);
            if (-1 == digit) {
                ADD_FAILURE()
                        << "cannot map format character to code: (" << c << ")";
                return -1;
            } else if (-2 == digit) {
            } else {
                result = (result * SEQ_STEP) + digit;
            }
            fmtflag = false;
        } else if ('%' == c) {
            fmtflag = true;
        }
        p++;
    }
    return result;
}

}  // namespace helper
}  // namespace test
}  // namespace recordlogs
