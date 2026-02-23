#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Loom.h"

namespace py = pybind11;

PYBIND11_MODULE(loom_python, m) {
    m.doc() = "Python bindings for the loom-qgis-plugin";

    // Expose the refactored main function
    m.def("run", &run, 
          R"doc(
Run the LOOM algorithm.

Args:
    args (list of str): A list containing two JSON strings:
        - The first string represents the graph in JSON format.
        - The second string represents the configuration in JSON format.

Returns:
    str: The optimized graph in JSON format.
)doc",
          py::arg("args"));
}