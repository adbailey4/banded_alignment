import unittest
from banded_alignment.bindings import banded_alignment


class TestBindings(unittest.TestCase):
    def test_banded_alignment(self):
        matches = banded_alignment("ATGC", "ATGC")
        self.assertEqual(matches, "4M")

    def test_long_banded_alignment(self):
        some_seq = "CATGTCCTGCAATGGCACTTAAGTACGTGCGCTTGCCATGGCG"
        some_other_seq = "CATGTCCTGCATGGCATTAGTACGTGGCTTGCCTGGCG"
        cigar = banded_alignment(some_seq, some_other_seq)
        self.assertEqual(cigar, "11M1D5M1D3M1D7M1D7M1D5M")


if __name__ == '__main__':
    unittest.main()
