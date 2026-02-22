#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Loom.h"

namespace py = pybind11;

PYBIND11_MODULE(loom_python, m) {
    m.doc() = "Python bindings for the loom-qgis-plugin";

    // Expose the refactored main function
    m.def("run", &run, "Run the algorithm function",py::arg("args"));
}