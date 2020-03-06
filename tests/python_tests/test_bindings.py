import unittest
from banded_alignment.bindings import banded_alignment


class TestBindings(unittest.TestCase):
    def test_banded_alignment(self):
        matches = banded_alignment("ATGC", "ATGC")
        self.assertEqual(matches, "4M")

    def test_long_banded_alignment(self):
        some_seq = "CATGTCCTGCATGGCATTAGTACGTGGCTTGCCTGGCG"
        some_other_seq = "AGATTGAACGTGTTGAACGTTGCCTGGCG"
        cigar = banded_alignment(some_other_seq, some_seq)
        self.assertEqual(cigar, "7M1I6M2I4M1I12M")


if __name__ == '__main__':
    unittest.main()
