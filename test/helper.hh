#ifndef TEST_RECORDLOGS_HELPER_HH
#define TEST_RECORDLOGS_HELPER_HH

#include <cstdint>

namespace recordlogs {
namespace test {
namespace helper {

int32_t get_format_character_code(char c);
int32_t caculate_format_sequence_code(const char *format);

}  // namespace helper
}  // namespace test
}  // namespace recordlogs

#endif /* TEST_RECORDLOGS_HELPER_HH */
