#include <cstdint>

#include <gtest/gtest.h>

#include "helper.hh"

namespace recordlogs {
namespace test {

using helper::caculate_format_sequence_code;
using helper::get_format_character_code;

TEST(GetFormatCharacterCodeTest, CharAccepted) {
    EXPECT_EQ(0, get_format_character_code('s'));
    EXPECT_EQ(1, get_format_character_code('d'));
}

TEST(GetFormatCharacterCodeTest, CharNoop) {
    EXPECT_EQ(-2, get_format_character_code('%'));
}

TEST(GetFormatCharacterCodeTest, CharReject) {
    EXPECT_EQ(-1, get_format_character_code('a'));
    EXPECT_EQ(-1, get_format_character_code('q'));
    EXPECT_EQ(-1, get_format_character_code('z'));
}

TEST(CaculateFormatSequenceCode, IncrementalChar) {
    EXPECT_EQ(12, caculate_format_sequence_code("%s"));
    EXPECT_EQ(13, caculate_format_sequence_code("%d"));
    EXPECT_EQ(49, caculate_format_sequence_code("%s%d"));
    EXPECT_EQ(196, caculate_format_sequence_code("%s%d%s"));
    EXPECT_EQ(785, caculate_format_sequence_code("%s%d%s%d"));
    EXPECT_EQ(785, caculate_format_sequence_code("%s%d%s%d%%"));
    EXPECT_EQ(785, caculate_format_sequence_code("%s%d%%%s%d"));
}

TEST(CaculateFormatSequenceCode, MessagePrefix) {
    EXPECT_EQ(785, caculate_format_sequence_code("[%s:%d] %s (errno=%d)"));
}

}  // namespace test
}  // namespace recordlogs
