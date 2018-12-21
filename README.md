# HighFive - HDF5 header-only C++ Library

[![Build Status](https://travis-ci.org/BlueBrain/HighFive.svg?branch=master)](https://travis-ci.org/BlueBrain/HighFive)

[![Coverity Statys](https://scan.coverity.com/projects/13635/badge.svg)](https://scan.coverity.com/projects/highfive)

## Brief

HighFive is a modern C++/C++11 friendly interface for libhdf5.

HighFive supports STL vector/string, Boost::UBLAS and Boost::Multi-array. It handles C++ from/to HDF5 automatic type mapping.
HighFive does not require an additional library and supports both HDF5 thread safety and Parallel HDF5 (contrary to the official hdf5 cpp)


### Design
- Simple C++-ish minimalist interface
- No other dependency than libhdf5
- Zero overhead
- Support C++11 ( compatible with C++98 )


### Dependencies
- libhdf5
- (optional) boost >= 1.41


### Usage

#### Write a std::vector<int> to 1D HDF5 dataset and read it back

```c++
using namespace HighFive;
// we create a new hdf5 file
File file("/tmp/new_file.h5", File::ReadWrite | File::Create | File::Truncate);

std::vector<int> data(50, 1);

// let's create a dataset of native integer with the size of the vector 'data'
DataSet dataset = file.createDataSet<int>("/dataset_one",  DataSpace::From(data));

// let's write our vector of int to the HDF5 dataset
dataset.write(data);

// read back
std::vector<int> result;
dataset.read(result);
```

> Note: if you can use `DataSpace::From` on your data, you can combine the create and write into one statement:
> 
> ```c++
> DataSet dataset = file.createDataSet("/dataset_one",  data);
> ```
>
> This works with `createAttribute`, as well.

#### Write a 2 dimensional C double float array to a 2D HDF5 dataset

See [create_dataset_double.cpp](src/examples/create_dataset_double.cpp)

#### Write and read a matrix of double float (boost::ublas) to a 2D HDF5 dataset

See [boost_ublas_double.cpp](src/examples/boost_ublas_double.cpp)

#### Write and read a subset of a 2D double dataset

See [select_partial_dataset_cpp11.cpp](src/examples/select_partial_dataset_cpp11.cpp)

#### Create, write and list HDF5 attributes

See [create_attribute_string_integer.cpp](src/examples/create_attribute_string_integer.cpp)

#### And others

See [src/examples/](src/examples/)  sub-directory for more infos

### Compile with HighFive

```bash
c++ -o program -I/path/to/highfive/include source.cpp  -lhdf5
```

#### H5Easy

For several 'standard' use cases the [HighFive/H5Easy.hpp](include/HighFive/H5Easy.hpp) interface is available. It allows:

*   Reading/writing in a single line of:

    -   scalars (to/from an extendible DataSet),
    -   strings,
    -   vectors (of standard types),
    -   [Eigen::Matrix](http://eigen.tuxfamily.org) (optional, enabled by including Eigen before including HighFive, or by `#define HIGHFIVE_EIGEN` before including HighFive),
    -   [xt::xarray](https://github.com/QuantStack/xtensor) and [xt::xtensor](https://github.com/QuantStack/xtensor) (optional, enabled by including xtensor before including HighFive, or by `#define HIGHFIVE_XTENSOR` before including HighFive).

*   Direct access to the `size` and `shape` of a DataSet.

Consider this example:

```cpp
#include <Eigen/Eigen>
#include <xtensor/xtensor.hpp>
#include <HighFive/H5Easy.hpp>

int main()
{
    HighFive::File file("example.h5", HighFive::File::Overwrite);

    // (over)write scalar
    {
        int A = 10;

        HighFive::dump(file, "/path/to/A", A);
        HighFive::dump(file, "/path/to/A", A, HighFive::Mode::Overwrite);
    }

    // (over)write std::vector
    {
        std::vector<double> B = {1., 2., 3.};

        HighFive::dump(file, "/path/to/B", B);
        HighFive::dump(file, "/path/to/B", B, HighFive::Mode::Overwrite);
    }

    // (over)write scalar in (automatically expanding) extendible DataSet
    {
        HighFive::dump(file, "/path/to/C", 10, {0});
        HighFive::dump(file, "/path/to/C", 11, {1});
        HighFive::dump(file, "/path/to/C", 12, {3});
    }

    // (over)write Eigen::Matrix
    {
        Eigen::MatrixXd D = Eigen::MatrixXd::Random(10,5);

        HighFive::dump(file, "/path/to/D", D);
        HighFive::dump(file, "/path/to/D", D, HighFive::Mode::Overwrite);
    }

    // (over)write xt::xtensor (or xt::xarray)
    {
        xt::xtensor<size_t,1> E = xt::arange<size_t>(10);

        HighFive::dump(file, "/path/to/E", E);
        HighFive::dump(file, "/path/to/E", E, HighFive::Mode::Overwrite);
    }

    // read scalar
    {
        int A = HighFive::load<int>(file, "/path/to/A");
    }

    // read std::vector
    {
        std::vector<double> B = HighFive::load<std::vector<double>>(file, "/path/to/B");
    }

    // read scalar from DataSet
    {
        int C = HighFive::load<int>(file, "/path/to/C", {0});
    }

    // read Eigen::Matrix
    {
        Eigen::MatrixXd D = HighFive::load<Eigen::MatrixXd>(file, "/path/to/D");
    }

    // read xt::xtensor (or xt::xarray)
    {
        xt::xtensor<size_t,1> E = HighFive::load<xt::xtensor<size_t,1>>(file, "/path/to/E");
    }

    // get the size/shape of a DataSet
    {
        size_t size = HighFive::size(file, "/path/to/C");
        std::vector<size_t> shape = HighFive::shape(file, "/path/to/C");
    }

    return 0;
}
```

### Test Compilation
Remember: Compilation is not required. Used only for unit test and examples

```bash
mkdir build; pushd build
cmake ../
make
make test
```

### Feature support

- create/read/write file,  dataset, group, dataspace.
- automatic memory management / ref counting
- automatic convertion of  std::vector and nested std::vector from/to any dataset with basic types
- automatic convertion of std::string to/from variable length string dataset
- selection() / slice support
- parallel Read/Write operations from several nodes with Parallel HDF5
- support HDF5 attributes


### Contributors
- Adrien Devresse <adrien.devresse@epfl.ch> - Blue Brain Project
- Ali Can Demiralp <demiralpali@gmail.com> -
- Fernando Pereira <fernando.pereira@epfl.ch> - Blue Brain Project
- Stefan Eilemann <stefan.eilemann@epfl.ch> - Blue Brain Project
- Tristan Carel <tristan.carel@epfl.ch> - Blue Brain Project
- Wolf Vollprecht <w.vollprecht@gmail.com> - QuantStack

### License
Boost Software License 1.0




