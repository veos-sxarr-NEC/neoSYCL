# neoSYCL

A SYCL Implementation for NEC SX-Aurora TSUBASA

## Requirements

- NEC Software Development Kit for Vector Engine
- g++ compiler supporting C++17
- Clang and LLVM 12 or higher (including development headers and shared
  libraries)
- Python 3
- CMake 3.10 or higher(required if building from source code)

## Configuration and Installation

- Install neoSYCL using rpm package of neoSYCL.

  nsc++ is installed into /opt/nec/ve/bin/nsc++.
  Commands and headers are installed into /opt/nec/ve/neosycl/bin and
  /opt/nec/ve/neosycl/include.

- If you want to build and install from neoSYCL source code the procedure is as follows.

```
download source code from https://github.com/veos-sxarr-NEC/neoSYCL
cd neoSYCL
mkdir build && cd build
cmake .. -DBUILD_KERNEL_OUTLINER=ON -DBUILD_VE=ON -DCMAKE_INSTALL_PREFIX=[your install path]
make 
make install
```
If Clang, LLVM or VEOS are installed to non-standard locations, their paths
should be given via the options `-DCLang_DIR=`, `-DLLVM_DIR=` and `-DVEO_DIR=`
options, respectively.

## Usage

The SYCL source file needs to be compiled using the `nsc++` compiler wrapper. 

nsc++ [source file]
./a.out

- nsc++ use kout command to generate host code and device code. The device
  code include the kernel routine.
- The host code works on VH and the device code works on VE.
- g++ compiles the host code and makes a.out.
- nc++ compiles the device code and transforms it into kernel-ve.so that is
  executed on VE through AVEO.

The device code which includes kernel routine can be executed on VH too.
In this case the nsc++ command requires "--device cpu" to be specified
as follows and the device code is compiled by g++.

nsc++ --device cpu [source file]
./a.out

## Example

A simple example code of neoSYCL is as follows.

#include <CL/sycl.hpp>
#include <vector>
#include <iostream>

using namespace cl::sycl;
using DATA_TYPE = float;

int main()
{

    int size = 500;
// Declaring data to be handled in a program.
    std::vector<DATA_TYPE> A(size * size,0.0);
    std::vector<DATA_TYPE> B(size * size,0.0);
    std::vector<DATA_TYPE> C(size * size,0.0);

    int icnt = 1;
    for(size_t i = 0 ; i < size ; i++){
        for(size_t j=0 ; j < size ; j++){
            B[i*size+j] = DATA_TYPE(icnt);
            C[i*size+j] = DATA_TYPE(icnt+icnt);
            icnt++;
        }
    }

// Start of scope. A sycl work must be enclosed in a scope({}) like this
// because the destructor of buffer have to be work at the end of the scope.
    {
        buffer<DATA_TYPE, 2> A_buffer(A.data(), range<2>(size, size));
        buffer<DATA_TYPE, 2> B_buffer(B.data(), range<2>(size, size));
        buffer<DATA_TYPE, 2> C_buffer(C.data(), range<2>(size, size));

// Create a queue for enqueue sycl work.
        queue testqueue;

// Submit a work.
        testqueue.submit([&](handler& cgh) {

// Setting access to buffer.
            auto Aary = A_buffer.get_access<access::mode::write>(cgh);
            auto Bary = B_buffer.get_access<access::mode::read>(cgh);
            auto Cary = C_buffer.get_access<access::mode::read>(cgh);

// Send parallel_for task to queue.
            cgh.parallel_for<class test2D>(range<2>(size, size), [=, size_ = size](item<2> item) {
                // Body of the kernel code
                Aary[item] = Bary[item] + Cary[item];
            }); // End of the kernel code
        });  // End of queue
        testqueue.wait();
    }
// End of scope. Memory is write backed from VE memory to VH memory.

// Print the result.
        std::cout << std::showpoint << "A.front=" << A.front()  << std::endl;
        std::cout << std::showpoint << "A.back=" << A.back()  << std::endl;

}
