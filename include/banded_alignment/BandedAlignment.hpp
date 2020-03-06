//
// Created by Andrew Bailey on 3/5/20.
//

#ifndef BANDED_ALIGNMENT_INCLUDE_BANDED_ALIGNMENT_BANDEDALIGNMENT_HPP_
#define BANDED_ALIGNMENT_INCLUDE_BANDED_ALIGNMENT_BANDEDALIGNMENT_HPP_

#include <string>
using namespace std;

string banded_alignment(string ref, string query);
char char_to_encoding(char base);
char const* string_to_encoding(string& sequence);

#endif //BANDED_ALIGNMENT_INCLUDE_BANDED_ALIGNMENT_BANDEDALIGNMENT_HPP_
