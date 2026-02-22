#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Add this line to enable STL type support
#include "Topo.h"

namespace py = pybind11;

PYBIND11_MODULE(topo_python, m) {
    m.doc() = "Python bindings for the loom-qgis-plugin";

    // Expose the refactored main function
    m.def("main", &main, "Run the main function", py::arg("args"));
}

