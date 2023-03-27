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

```
nsc++ [source file]
./a.out
```

- nsc++ use kout command to generate host code and device code. The device
  code include the kernel routine.
- The host code works on VH and the device code works on VE.
- g++ compiles the host code and makes a.out.
- nc++ compiles the device code and transforms it into kernel-ve.so that is
  executed on VE through AVEO.

The device code which includes kernel routine can be executed on VH too.
In this case the nsc++ command requires "--device cpu" to be specified
as follows and the device code is compiled by g++.

```
nsc++ --device cpu [source file]
./a.out
```

## Example

A simple example of neoSYCL is as follows.

- Source code

```
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
```

- Compile

```
$ nsc++ test.cpp
```

- Execution

```
$ ./a.out
A.front=3.00000
A.back=750000.
```

## API list

This is a list of APIs available in neoSYCL.
<br>
The API list of the SYCL implementation is as follows:https://www.khronos.org/files/sycl/sycl-121-reference-card.pdf

### Device selection class

In addition to all APIs, there are the following neoSYCL-specific device selectors:
  * ve_selector

### Platform class

  * platform();
  * explicit platform(const device\_selector &deviceSelector);
  * static vector\_class<platform> get\_platforms();
  * template <info::platform param> typename info::param\_traits<info::platform,param>::return\_type get\_info() const;
  * bool has\_extension(const string\_class &extension) const;
  * bool is\_host() const;

### Context class

  * explicit context(async\_handler asyncHandler = {});
  * context(const device &dev,async\_handler asyncHandler = {});
  * vector\_class<device> get\_devices() const;

### Device class

  * device();
  * explicit device(const device\_selector &deviceSelector);
  * platform get\_platform() const;
  * bool is\_host() const;
  * bool is\_cpu() const;
  * bool is\_gpu() const;
  * bool is\_accelerator() const;
  * bool has\_extension(const string\_class &extension) const;
  * static vector\_class<device> get\_devices(info::device\_type deviceType = info::device\_type::all);

### Queue class

  * explicit queue(const property\_list &propList = {});
  * queue(const async\_handler &asyncHandler,const property\_list &propList = {});
  * queue(const device\_selector &deviceSelector,const property\_list &propList = {});
  * queue(const device\_selector &deviceSelector,const async\_handler &asyncHandler,const property\_list &propList = {});
  * queue(const device &syclDevice,const property\_list &propList = {});
  * queue(const device &syclDevice,const async\_handler &asyncHandler,const property\_list &propList = {});
  * context get\_context() const;
  * device get\_device() const;
  * bool is\_host();
  * void wait();
  * void wait\_and\_throw();
  * void throw\_asynchronous();
  * template <typename T> event submit(T cgf);

### Buffer class

Class Declaration
  * template <typename T,int dimensions = 1,typename AllocatorT = cl::sycl::buffer_allocator> class buffer

Member Functions
  * buffer(const range<dimensions> &bufferRange,const property\_list &propList = {});
  * buffer(const range<dimensions> &bufferRange,AllocatorT allocator,const property\_list &propList = {});
  * buffer(T \*hostData,const range<dimensions> &bufferRange,const property\_list &propList = {});
  * buffer(T \*hostData,const range<dimensions> &bufferRange,AllocatorT allocator,const property\_list &propList = {});
  * buffer(const T \*hostData,const range<dimensions> &bufferRange,const property\_list &propList = {});
  * buffer(const T \*hostData,const range<dimensions> &bufferRange,AllocatorT allocator,const property\_list &propList = {});
  * buffer(shared\_ptr\_class<T> &hostData,const range<dimensions> &bufferRange,AllocatorT allocator,const property\_list &propList = {});
  * buffer(shared\_ptr\_class<T> &hostData,const range<dimensions> &bufferRange,const property\_list &propList = {});
  * template <class Inputiterator> buffer(Inputiterator first,Inputiterator last,AllocatorT allocator,const property\_list &propList = {});
  * template <class Inputiterator> buffer(Inputiterator first,Inputiterator last,const property\_list &propList = {});
  * buffer(buffer<T,dimensions,AllocatorT> &b,const index<dimensions> &baseIndex,const index<dimensions> &subRange);
  * range<dimensions> get\_range() const;
  * size\_t get\_count() const;
  * size\_t get\_size() const;
  * AllocatorT get\_allocator() const;
  * template <access::mode mode,access::target target = access::target::global_buffer> accessor<T,dimensions,mode,target> get\_access(handler &commandGroupHandler);
  * template <access::mode mode> accessor<T,dimensions,mode,access::target::host_buffer> get\_access();
  * template <access::mode mode,access::target target = access::target::global_buffer> accessor<T,dimensions,mode,target> get\_access(handler &commandGroupHandler,range<dimensions> accessRange,id<dimensions> accessOffset = {});
  * template <access::mode mode> accessor<T,dimensions,mode,access::target::host_buffer> get\_access(range<dimensions> accessRange,id<dimensions> accessOffset = {});
  * template <typename Destination = std::nullptr_t> void set\_final\_data(Destination finalData = std::nullptr);
  * template <typename reinterpretT,int reinterpretDim> buffer<reinterpretT,reinterpretDim,AllocatorT> reinterpret(range<reinterpretDim> reinterpretrange) const;

### Event class

  * event();
  * void wait();
  * void wait\_and\_throw();

### Accessor class

Enums access::mode
  * read
  * write
  * read\_write
  * discard\_write
  * discard\_read\_write

Enums access::target
  * global\_buffer
  * constant\_buffer
  * host\_buffer

Class Declaration
  * templete <typename dataT,int dimensions,access::mode accessmode,access::target accessTarget = access::target::global\buffer,access::placeholder isPlaceholder = access::placeholder::false_t> class accessor;

Members of class accessor for buffers
  * accessor(buffer<dataT,1> &bufferRef);
  * accessor(buffer<dataT,1> &bufferRef,handler &commandGroupHandlerRef);
  * accessor(buffer<dataT,dimensions> &bufferRef);
  * accessor(buffer<dataT,dimensions> &bufferRef,handler &commandGroupHandlerRef);
  * accessor(buffer<dataT,dimensions> &bufferRef,range<dimensions> accessRange,id<dimensions> accessOffset = {});
  * accessor(buffer<dataT,dimensions> &bufferRef,handler &commandGroupHandlerRef,range<dimensions> accessRange,id<dimensions> accessOffset = {});
  * constexpt bool is\_placeholder() const;
  * size\_t get\_size() const;
  * size\_t get\_count() const;
  * range<dimensions> get\_range() const;
  * range<dimensions> get\_offset() const;
  * operator dataT &() const;
  * dataT &operator[](id<dimensions> index) const;
  * dataT &operator[](size\_t index) const;
  * operator dataT() const;
  * dataT operator[](id<dimensions> index) const;
  * dataT operator[](size\_t index) const;
  * dataT \*get\_pointer() const;

### Range class

Class Declaration
  * template <size\_t dimensions = 1> class range;

Member functions
  * range(size\_t dim0);
  * range(size\_t dim0,size\_t dim1);
  * range(size\_t dim0,size\_t dim1,size\_t dim2);
  * size\_t get(int dimension) const;
  * size\_t &operator[](int dimension);
  * size\_t size() const;
  * range<dimensions> operatorOP(const range<dimensions> &rhs) const;
  * range<dimensions> operatorOP(const size\_t &rhs) const;
  * range<dimensions> &operatorOP(const range<dimensions> &rhs);
  * range<dimensions> &operatorOP(const size\_t &rhs);

### Id class

Class Declaration
  * template <size\_t dimensions = 1> struct id;

Member functions
  * id();
  * id(size\_t dim0);
  * id(size\_t dim0,size\_t dim1);
  * id(size\_t dim0,size\_t dim1,size\_t dim2);
  * id(const range<dimensions> &range);
  * id(const item<dimensions> &item);
  * size\_t get(int dimension) const;
  * size\_t &operator[](int dimension) const;
  * operator size\_t();
  * id<dimensions> operatorOP(const id<dimensions> &rhs) const;
  * id<dimensions> operatorOP(const size\_t &rhs) const;
  * id<dimensions> &operatorOP(const id<dimensions> &rhs);
  * id<dimensions> &operatorOP(const size\_t &rhs);

### Item class

Class Declaration
  * template <int dimensions = 1,bool with_offset = true> struct item;

Member functions
  * id<dimensions> get\_id() const;
  * size\_t get\_id(int dimension) const;
  * size\_t &operator[](int dimension);
  * range<dimensions> get\_range() const;
  * id<dimensions> get\_offset() const;
  * size\_t get\_linear\_id() const;

### Command group class handler

  * template <typename KernelName,typename KernelType> void single\_task(KernelType kernelFunc)
  * template <typename KernelName,typename KernelType,int dimensions> void parallel\_for(range<dimensions> numWorkItems,KernelType kernelFunc);
  * template <typename KernelName,typename KernelType,int dimensions> void parallel\_for(range<dimensions> numWorkItems,id<dimensions> workItemOffset,KernelType kernelFunc);
  * template <typename T,int dim,access::mode mode,access::target tgt> void copy(accessor<T,dim,mode,tgt> src,shared\_ptr\_class<T> dest);
  * template <typename T,int dim,access::mode mode,access::target tgt> void copy(shared\_ptr\_class<T> src, accessor<T,dim,mode,tgt> dest);
  * template <typename T,int dim,access::mode mode,access::target tgt> void copy(accessor<T,dim,mode,tgt> src,T \*dest);
  * template <typename T,int dim,access::mode mode,access::target tgt> void copy(T \*src,accessor<T,dim,mode,tgt> dest);
  * template <typename T,int dim,access::mode mode,access::target tgt> void copy(accessor<T,dim,mode,tgt> src,accessor<T,dim,mode,tgt> dest);
  * template <typename T,int dim,access::mode mode,access::target tgt> void update\_host(accessor<T,dim,mode,tgt> acc);
  * template <typename T,int dim,access::mode mode,access::target tgt> void fill(accessor<T,dim,mode,tgt> dest,const T& src);

### Kernel class

  * bool is\_host() const;
  * context get\_context() const;
  * program get\_program() const;

### Program class

  * explicit program(const context &context);
  * program(const context &context,vector\_class<device> deviceList);
  * bool is\_host() const;
  * template <typename kernelT> bool has\_kernel<kernelT>() const;
  * bool has\_kernel(string\_class kernelName) const;
  * template <typename kernelT> bool get\_kernel<kernelT>() const;
  * kernel get\_kernel(string\_class kernelName) const;
  * context get\_context() const;
  * vector\_class<device> get\_devices() const;
  * program\_state get\_state() const;

### Scalar data types

neoSYCL supports the C++11 ISO standard data types.

### Vector data types

Class declaration
  * template <typename dataT,int numElements> class vec;

Members
  * using element\_type = dataT;
  * vec();
  * vec(const vec<dataT,numElements> &rhs);
  * operator dataT() const;
  * size\_t get\_count() const;
  * size\_t get\_size() const;
  * template <<typename convertT,rounding\_mode>>
  * vec<dataT,numElements> &operator=(const vec<dataT,numElements> &rhs);
  * vec<dataT,numElements> &operator=(const dataT &rhs);
  * vec<RET,numElements> operator!();
  * vec<dataT,numElements> operator~();

### Exception classes

Class:exception
  * const char \*what() const;
  * context get\_context() const;
  * bool has\_context() const;

Class:exception\_list
  * size\_t size() const;
  * iterator begin() const;
  * iterator end() const;

### Math functions

All APIs are supported.

### Integer functions

All APIs are supported except for the following

  * Tint32bit mad24(Tint32bit x,Tint32bit y,Tint32bit Z)
  * Tint32bit mul24(Tint32bit x,Tint32bit y)

### Common functions

All APIs are supported.

### Geometric functions

All APIs are supported.
