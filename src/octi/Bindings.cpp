#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Add this line to enable STL type support
#include "Octi.h"

namespace py = pybind11;

PYBIND11_MODULE(octi_python, m) {
    m.doc() = "Python bindings for the loom-qgis-plugin";

    // Expose the refactored main function
    m.def(
        "run_octi", 
        &run_octi, 
        R"doc(
            Run the algorithm function.

            Args:
                args (list[str]): A list of arguments to pass to the algorithm.

            Returns:
                int: The result of the algorithm execution.
        )doc",
        py::arg("args")
    );
}

