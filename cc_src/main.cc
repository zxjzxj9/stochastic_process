#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <utility>
#include <algorithm>

namespace py = pybind11;
using range = std::pair<float, float>;
using coord = std::pair<float, float>;

struct Circle_r1 {
    static bool isBound(const coord& c) {
        return c.first*c.first + c.second*c.second >= 1.00;
    }
};

template<typename Fp>
class MeshGrid {
public:
    // m, n the number of grids in x and y directions
    // xr range (xmin, xmax)
    // yr range (ymin, ymax)
    MeshGrid(int m, int n, float dt, range xr, range yr):
            m(m), n(n), dt(dt), xr(xr), yr(yr) {
        data = new float[m*n];
        mask = new bool[m*n]; // mask the boundary

        std::fill_n(data, m*n, 0);
        // initialize the boundary
        for (int i=0; i<m; i++) {
            for(int j=0; j <n; j++) {
                auto c = get_coord(i, j);
                if(Fp::isBound(c)) {
                    mask[i*n+j] = false;
                    data[i*n+j] = 1.0;
                }
                else mask[i*n + j] = true;
            }
        }

        dx = (xr.second - xr.first)/(m-1);
        dy = (yr.second - yr.first)/(n-1);

    }

    ~MeshGrid() {
        delete[] data;
        delete[] mask;
    }

    float& operator()(int i, int j) {
        if (i>=m || j >= n) throw std::runtime_error("Error: index exceeded...");
        return data[i*n + j];
    }

    coord get_coord(int i, int j) {
        if (i>=m || j >= n) throw std::runtime_error("Error: index exceeded...");
        return {xr.first + (xr.second - xr.first)*i/(m-1), yr.first + (yr.second - yr.first)*i/(m-1)};
    }

    // step dt for pde \frac{\partial{T}}{\partial{t}} = -0.5*\nebla^2 T
    void step() {
        auto self = *this;
        for (int i=0; i<m; i++) {
            for(int j=0; j<n; j++) {
                if(i+1>=m || i-1<=0) continue;
                if(j+1>=n || j-1<=0) continue;
                if(mask[i*n + j]) {
                    auto dx2 = (self(i+1, j) + self(i-1, j) - 2*self(i, j))/(dx*dx);
                    auto dy2 = (self(i, j+1) + self(i, j-1) - 2*self(i, j))/(dy*dy);
                    self(i, j) += -0.5*(dx2+dy2)*dt;
                }
            }
        }
    }

    float* get_data() {
        return data;
    }

    int rows() {return m;}
    int cols() {return n;}

private:
    int m;
    int n;
    range xr;
    range yr;
    Fp bound;
    float dt;
    float dx;
    float dy;
    float* data;
    bool* mask;
};

// circle mesh grid
using MG1 = MeshGrid<Circle_r1>;

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(heat, m) {
    m.doc() = "Heat transportation equestion"; // optional module docstring
    py::class_<MG1>(m, "MeshCircle", py::buffer_protocol())
            .def(py::init<int, int, float, range, range>())
            .def("step", &MG1::step)
            .def_buffer([](MG1& m)-> py::buffer_info {
                return py::buffer_info(
                        m.get_data(),
                        sizeof(float),
                        py::format_descriptor<float>::format(),
                        2,
                        {m.rows(), m.cols()},
                        { sizeof(float) * m.cols(), sizeof(float)}
                );
            });
}
