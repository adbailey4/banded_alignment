//
// Created by Andrew Bailey on 03/05/20.
//

// banded_alignment
#include <banded_alignment/BandedAlignment.hpp>
// gtest
#include <gtest/gtest.h>
#include <gmock/gmock.h>



TEST (BandedAlignmentTests, test_char_to_encoding) {
  char base = char_to_encoding('A');
  EXPECT_EQ(base, 0x01);
  base = char_to_encoding('C');
  EXPECT_EQ(base, 0x02);
  base = char_to_encoding('G');
  EXPECT_EQ(base, 0x04);
  base = char_to_encoding('T');
  EXPECT_EQ(base, 0x08);
}

TEST (BandedAlignmentTests, test_string_to_encoding) {
  string test = "ACGT";
  char const* bases = string_to_encoding(test);
  EXPECT_EQ(bases[0], 0x01);
  EXPECT_EQ(bases[1], 0x02);
  EXPECT_EQ(bases[2], 0x04);
  EXPECT_EQ(bases[3], 0x08);
  free((void *) bases);
}

TEST (BandedAlignmentTests, test_banded_alignment){
  string matches = banded_alignment("ATATAGCATTTT", "ATGCATTTT");
  EXPECT_EQ(matches, "2M3D7M");
  string matches2 = banded_alignment("CATGTCCTGCAATGGCACTTAAGTACGTGCGCTTGCCATGGCG", "CATGTCCTGCATGGCATTAGTACGTGGCTTGCCTGGCG");
  EXPECT_EQ(matches2, "11M1D5M1D3M1D7M1D7M1D5M");
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
