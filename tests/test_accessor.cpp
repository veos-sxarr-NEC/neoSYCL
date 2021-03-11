#include "gtest/gtest.h"
#include "CL/sycl.hpp"

using namespace cl::sycl;


TEST(accessor, basic_tests) {
  size_t X = 2;
  size_t Y = 10;
  float val[X * Y];

  for (size_t i = 0; i < X * Y; i++) {
    val[i] = i;
  }

  buffer<float, 2> buf(val, range<2>(X, Y));

  auto acc = buf.get_access<access::mode::read_write>();

  EXPECT_EQ(acc[id<2>(0, 0)], 0);
}

TEST(accessor, buffer_2d) {
  buffer<float, 2> buf(range<2>(2, 3));

  auto acc = buf.get_access<access::mode::read_write>();

  acc[id<2>(1, 1)] = 233;

  EXPECT_EQ(acc[id<2>(1, 1)], 233);
  EXPECT_EQ(acc[1][1], 233);
  EXPECT_EQ(acc.get_range(), range<2>(2, 3));
}

TEST(accessor, buffer_3d) {
  buffer<float, 3> buf(range<3>(2, 3, 3));

  auto acc = buf.get_access<access::mode::read_write>();

  acc[id<3>(1, 1, 1)] = 233;

  EXPECT_EQ(acc[id<3>(1, 1, 1)], 233);
  EXPECT_EQ(acc[1][1][1], 233);
  EXPECT_EQ(acc.get_range(), range<3>(2, 3, 3));
}
