#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Loom.h"

namespace py = pybind11;

void register_loom(py::module &m) {
    m.def("run_loom", &run_loom, py::arg("args"));
}