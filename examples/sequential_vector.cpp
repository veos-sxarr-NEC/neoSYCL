#include <CL/sycl.hpp>
#include <iostream>
#include <unistd.h>

using namespace cl::sycl;

int main() {
  const int N = 3;
  using Vector = float[N];

  Vector a = {1, 2, 3};
  Vector b = {5, 6, 8};

  float c[N];

  { // By sticking all the SYCL work in a {} block, we ensure
    // all SYCL tasks must complete before exiting the block

    // Create a queue to work on
    queue myQueue;

    // Create buffers from a & b vectors with 2 different syntax
    buffer<float> A(a, range<1>(N));
    buffer<float> B(b, range<1>(N));

    // A buffer of N float using the storage of c
    buffer<float> C(c, N);

    /* The command group describing all operations needed for the kernel
       execution */
    myQueue.submit([&](handler &cgh) {
      // In the kernel A and B are read, but C is written
      auto ka = A.get_access<access::mode::read>(cgh);
      auto kb = B.get_access<access::mode::read>(cgh);
      auto kc = C.get_access<access::mode::write>(cgh);

      // Enqueue a single, simple task
      cgh.single_task<class sequential_vector>([=]() {
        for (size_t i = 0; i != N; i++) {
          kc[i] = ka[i] + kb[i];
        }
      });
    }); // End of our commands for this queue

  } // End scope, so we wait for the queue to complete

  std::cout << "Result:" << std::endl;
  for (size_t i = 0; i != N; i++)
    std::cout << c[i] << " ";
  std::cout << std::endl;
}
