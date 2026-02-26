#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// forward declarations from other Bindings.cpp
void register_topo(pybind11::module &m);
void register_octi(pybind11::module &m);
void register_loom(pybind11::module &m);



namespace py = pybind11;

PYBIND11_MODULE(loom, m) {
    m.doc() = "Python bindings for loom";

    register_topo(m);
    register_octi(m);
    register_loom(m);

}