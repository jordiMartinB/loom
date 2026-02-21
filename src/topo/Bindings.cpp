#include <pybind11/pybind11.h>
#include "TopoMain.h"

namespace py = pybind11;

PYBIND11_MODULE(loom, m) {
    m.doc() = "Python bindings for the loom-qgis-plugin";

    // Expose the refactored main function
    m.def("run_main", &run_main, "Run the main function",
          py::arg("args"));
}

