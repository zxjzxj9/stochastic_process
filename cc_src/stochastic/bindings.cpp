#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "rng.h"
#include "process.h"
#include "payoff.h"
#include "mc.h"

namespace py = pybind11;

// Helper: convert vector<vector<double>> to 2-D numpy array (n_paths x path_len)
static py::array_t<double> to_numpy_2d(const std::vector<std::vector<double>>& paths) {
    if (paths.empty()) return py::array_t<double>(std::vector<size_t>{0, 0});
    size_t nrows = paths.size();
    size_t ncols = paths[0].size();
    py::array_t<double> arr({nrows, ncols});
    auto buf = arr.mutable_unchecked<2>();
    for (size_t i = 0; i < nrows; ++i)
        for (size_t j = 0; j < ncols; ++j)
            buf(i, j) = paths[i][j];
    return arr;
}

PYBIND11_MODULE(stochastic, m) {
    m.doc() = "Stochastic process simulation library for quantitative finance";

    // --------------------------------------------------------
    // MCResult
    // --------------------------------------------------------
    py::class_<MCResult>(m, "MCResult")
        .def_readonly("price",     &MCResult::price)
        .def_readonly("std_error", &MCResult::std_error)
        .def_readonly("n_paths",   &MCResult::n_paths)
        .def("__repr__", [](const MCResult& r) {
            return "MCResult(price=" + std::to_string(r.price) +
                   ", std_error=" + std::to_string(r.std_error) +
                   ", n_paths=" + std::to_string(r.n_paths) + ")";
        });

    // --------------------------------------------------------
    // RNG
    // --------------------------------------------------------
    py::class_<RNG>(m, "RNG")
        .def(py::init<unsigned>(), py::arg("seed") = 42)
        .def("seed",          &RNG::seed)
        .def("normal",        py::overload_cast<>(&RNG::normal))
        .def("normal",        py::overload_cast<double, double>(&RNG::normal),
             py::arg("mean"), py::arg("stddev"))
        .def("poisson",       &RNG::poisson,  py::arg("lambda_"))
        .def("uniform",       &RNG::uniform)
        .def("normal_vector", &RNG::normal_vector, py::arg("n"));

    // --------------------------------------------------------
    // WienerProcess
    // --------------------------------------------------------
    py::class_<WienerProcess>(m, "WienerProcess")
        .def(py::init<>())
        .def("simulate_terminal", &WienerProcess::simulate_terminal,
             py::arg("T"), py::arg("rng"))
        .def("simulate_path", [](const WienerProcess& self, double T, int n_steps, RNG& rng) {
                return self.simulate_path(T, n_steps, rng);
             }, py::arg("T"), py::arg("n_steps"), py::arg("rng"))
        .def("simulate_paths", [](const WienerProcess& self, double T, int n_steps, int n_paths, RNG& rng) {
                return to_numpy_2d(self.simulate_paths(T, n_steps, n_paths, rng));
             }, py::arg("T"), py::arg("n_steps"), py::arg("n_paths"), py::arg("rng"));

    // --------------------------------------------------------
    // GBMProcess
    // --------------------------------------------------------
    py::class_<GBMProcess>(m, "GBMProcess")
        .def(py::init<double, double, double>(),
             py::arg("S0"), py::arg("r"), py::arg("sigma"))
        .def("simulate_terminal", &GBMProcess::simulate_terminal,
             py::arg("T"), py::arg("rng"))
        .def("simulate_path", [](const GBMProcess& self, double T, int n_steps, RNG& rng) {
                return self.simulate_path(T, n_steps, rng);
             }, py::arg("T"), py::arg("n_steps"), py::arg("rng"))
        .def("simulate_paths", [](const GBMProcess& self, double T, int n_steps, int n_paths, RNG& rng) {
                return to_numpy_2d(self.simulate_paths(T, n_steps, n_paths, rng));
             }, py::arg("T"), py::arg("n_steps"), py::arg("n_paths"), py::arg("rng"))
        .def_property_readonly("S0",    &GBMProcess::get_S0)
        .def_property_readonly("r",     &GBMProcess::r)
        .def_property_readonly("sigma", &GBMProcess::get_sigma);

    // --------------------------------------------------------
    // MertonProcess
    // --------------------------------------------------------
    py::class_<MertonProcess>(m, "MertonProcess")
        .def(py::init<double, double, double, double, double, double>(),
             py::arg("S0"), py::arg("r"), py::arg("sigma"),
             py::arg("lam"), py::arg("mu_J"), py::arg("sigma_J"))
        .def("compensator",       &MertonProcess::compensator)
        .def("simulate_terminal", &MertonProcess::simulate_terminal,
             py::arg("T"), py::arg("rng"))
        .def("simulate_path", [](const MertonProcess& self, double T, int n_steps, RNG& rng) {
                return self.simulate_path(T, n_steps, rng);
             }, py::arg("T"), py::arg("n_steps"), py::arg("rng"))
        .def("simulate_paths", [](const MertonProcess& self, double T, int n_steps, int n_paths, RNG& rng) {
                return to_numpy_2d(self.simulate_paths(T, n_steps, n_paths, rng));
             }, py::arg("T"), py::arg("n_steps"), py::arg("n_paths"), py::arg("rng"))
        .def_property_readonly("S0",      &MertonProcess::get_S0)
        .def_property_readonly("r",       &MertonProcess::r)
        .def_property_readonly("sigma",   &MertonProcess::get_sigma)
        .def_property_readonly("lam",     &MertonProcess::get_lam)
        .def_property_readonly("mu_J",    &MertonProcess::get_mu_J)
        .def_property_readonly("sigma_J", &MertonProcess::get_sigma_J);

    // --------------------------------------------------------
    // Payoff classes
    // --------------------------------------------------------
    py::class_<Payoff>(m, "Payoff")
        .def("evaluate", &Payoff::evaluate, py::arg("path"));

    py::class_<EuropeanCall, Payoff>(m, "EuropeanCall")
        .def(py::init<double>(), py::arg("K"));

    py::class_<EuropeanPut, Payoff>(m, "EuropeanPut")
        .def(py::init<double>(), py::arg("K"));

    py::class_<DigitalCall, Payoff>(m, "DigitalCall")
        .def(py::init<double>(), py::arg("K"));

    py::class_<AsianCall, Payoff>(m, "AsianCall")
        .def(py::init<double>(), py::arg("K"));

    py::class_<AsianPut, Payoff>(m, "AsianPut")
        .def(py::init<double>(), py::arg("K"));

    py::class_<UpAndOutCall, Payoff>(m, "UpAndOutCall")
        .def(py::init<double, double>(), py::arg("K"), py::arg("barrier"));

    py::class_<DownAndOutPut, Payoff>(m, "DownAndOutPut")
        .def(py::init<double, double>(), py::arg("K"), py::arg("barrier"));

    py::class_<FloatingLookbackCall, Payoff>(m, "FloatingLookbackCall")
        .def(py::init<>());

    py::class_<FloatingLookbackPut, Payoff>(m, "FloatingLookbackPut")
        .def(py::init<>());

    py::class_<FixedLookbackCall, Payoff>(m, "FixedLookbackCall")
        .def(py::init<double>(), py::arg("K"));

    py::class_<FixedLookbackPut, Payoff>(m, "FixedLookbackPut")
        .def(py::init<double>(), py::arg("K"));

    // --------------------------------------------------------
    // MC pricing functions
    // --------------------------------------------------------
    m.def("mc_price_gbm",
        [](const GBMProcess& proc, const Payoff& payoff,
           double T, int n_steps, int n_paths, unsigned seed) {
            return mc_price(proc, payoff, T, n_steps, n_paths, seed);
        },
        py::arg("process"), py::arg("payoff"),
        py::arg("T"), py::arg("n_steps"), py::arg("n_paths"),
        py::arg("seed") = 42,
        "Price an option on a GBM process via Monte Carlo.");

    m.def("mc_price_merton",
        [](const MertonProcess& proc, const Payoff& payoff,
           double T, int n_steps, int n_paths, unsigned seed) {
            return mc_price(proc, payoff, T, n_steps, n_paths, seed);
        },
        py::arg("process"), py::arg("payoff"),
        py::arg("T"), py::arg("n_steps"), py::arg("n_paths"),
        py::arg("seed") = 42,
        "Price an option on a Merton jump-diffusion process via Monte Carlo.");
}
