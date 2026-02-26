#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Topo.h"

namespace py = pybind11;

void register_topo(py::module &m) {
    // Updated module description
    m.doc() = R"pbdoc(
        Python bindings for the Topo module in the loom-qgis-plugin.

        This module provides access to the core functionality of the Topo library,
        allowing Python scripts to interact with and execute the `run_topo` function
        of the Topo module. The `run_topo` function accepts arguments as a list of strings
        and processes them accordingly.

        Example usage:
            import topo_python
            result = topo_python.run_topo(["arg1", "arg2", "arg3"])
    )pbdoc";

    // Expose the refactored run_topo function
    m.def("run_topo", &run_topo, 
          R"pbdoc(
              Execute the Topo algorithm with the specified arguments.

              Args:
                  args (list of str): A list of arguments to pass to the Topo algorithm.

              Returns:
                  The result of the algorithm execution.

              Example:
                  result = topo_python.run(["arg1", "arg2", "arg3"])
          )pbdoc", 
          py::arg("args"));
}

