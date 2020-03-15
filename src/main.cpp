//
// Created by Andrew Bailey on 3/5/20.
//

#include <banded_alignment/BandedAlignment.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
  string cigar = banded_alignment("ATGC", "ATGC");
  cout << cigar << "\n";
  return 0;
}
