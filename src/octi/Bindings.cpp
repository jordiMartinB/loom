#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Octi.h"

namespace py = pybind11;

void register_octi(py::module &m) {
    m.def("run_octi", &run_octi, py::arg("args"));
}