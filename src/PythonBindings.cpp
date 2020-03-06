//
// Created by Andrew Bailey on 03/05/20.
//

#ifndef BANDED_ALIGNMENT_PYBIND_API_H
#define BANDED_ALIGNMENT_PYBIND_API_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <banded_alignment/BandedAlignment.hpp>
using namespace pybind11;


PYBIND11_MODULE(bindings, module) {
  module.doc() = R"pbdoc(
        Banded Alignment:
        - banded_alignment
    )pbdoc";

  module.def("banded_alignment", &banded_alignment, R"pbdoc(
  - Generate cigar string between two a
 @param ref: reference sequence
 @param query: query sequence

    )pbdoc",
    pybind11::arg("ref"),
    pybind11::arg("query"));

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  module.attr("__version__") = "dev";
#endif
}

#endif
