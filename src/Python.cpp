#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "src/loom/Loom.h"
#include "src/topo/Topo.h"
#include "src/octi/Octi.h"
#include "src/transitmap/Transitmap.h"



namespace py = pybind11;

PYBIND11_MODULE(loom, m) {
    m.doc() = R"doc(
        Python bindings for the loom transit map rendering toolkit.

        This module exposes the core loom pipeline stages as callable functions.
        Each function accepts a list of command-line-style string arguments, mirroring
        the behaviour of the corresponding command-line tools.

        Typical pipeline order:
            1. run_topo   – clean and topologise raw transit network data
            2. run_loom   – compute line orderings on shared edges
            3. run_octi   – produce an octilinear graph layout
            4. run_transitmap – render the final transit map (SVG/PDF)
    )doc";

    m.def("run_loom", &run_loom, py::arg("args"),
        R"doc(
            Run the loom line-ordering stage.

            Reads a topologised transit graph from stdin and writes a graph with
            optimised line orderings on shared edges to stdout.

            Args:
                args (list[str]): Command-line arguments forwarded to loom
                                  (e.g. ["-v", "--optim-passes", "3"]).

            Returns:
                int: Exit code (0 on success).
        )doc");

    m.def("run_topo", &run_topo, py::arg("args"),
        R"doc(
            Run the topo topologisation stage.

            Reads raw GTFS-derived or GeoJSON transit data from stdin, cleans
            duplicate edges and shared segments, and writes a topologised graph
            to stdout.

            Args:
                args (list[str]): Command-line arguments forwarded to topo
                                  (e.g. ["--snap-dist", "20"]).

            Returns:
                int: Exit code (0 on success).
        )doc");

    m.def("run_octi", &run_octi, py::arg("args"),
        R"doc(
            Run the octi octilinear layout stage.

            Reads a loom-ordered graph from stdin and computes an octilinear
            (8-directional) embedding of the network, writing the result to stdout.

            Args:
                args (list[str]): Command-line arguments forwarded to octi
                                  (e.g. ["--grid-size", "50"]).

            Returns:
                int: Exit code (0 on success).
        )doc");

    m.def("run_transitmap", &run_transitmap, py::arg("args"),
        R"doc(
            Run the transitmap rendering stage.

            Reads an octilinear graph from stdin and renders a styled transit map,
            writing SVG or PDF output to stdout.

            Args:
                args (list[str]): Command-line arguments forwarded to transitmap
                                  (e.g. ["--svg", "--line-width", "2"]).

            Returns:
                int: Exit code (0 on success).
        )doc");
}